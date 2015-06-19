#include <stats.h>

 
uint8_t udp_hdr_size = 8;
uint8_t tcp_hdr_size = 32; 
uint8_t ip_hdr_size=20;
uint8_t eth_hdr_size=20;
uint32_t test_runtime = 0; //TODO init file, initate the testruntime
uint64_t pps_limit = 0; //TODO
uint64_t pps_delay = 0; //TODO
struct average_filter *root_rtt_average = NULL;
struct average_filter *end_rtt_average = NULL;
const uint8_t average_filter_len = 255;
struct print_stats *root_test_stats = NULL;
struct print_stats *end_test_stats  = NULL;
struct rte_eth_stats* port_stats;
uint64_t stats_time_period=0;
uint8_t average_list_size = 0;

void clearScr(void)
{
        const char clr[] = { 27, '[', '2', 'J', '\0' };
        const char topLeft[] = { 27, '[', '1', ';', '1', 'H','\0' };

        // Clear screen and move to top left
        printf("%s%s", clr, topLeft);
}

int init_stats (uint8_t port_id, uint32_t dst_ip ) {
    
    uint8_t socket_id = rte_eth_dev_socket_id(port_id);
    port_stats= rte_malloc ("PortStatArray", sizeof(struct rte_eth_stats)*total_eth_ports, 0 );
    
    struct rte_mbuf * m = rte_pktmbuf_alloc ( tx_mempool[socket_id] );
    rte_pktmbuf_prepend(m, sizeof(struct packet_stats));
	struct packet_stats *sta  = rte_pktmbuf_mtod(m, struct packet_stats *);
    sta->PPS = 512; //randomly written PPS packet persecond
    sta->timestamp = rte_cpu_to_be_64(rte_rdtsc()/one_usec);
    
    struct udp_conn_t* udp_stat_struct = rte_malloc ("UdpStructurForStat", sizeof (struct udp_conn_t ), 0 );
    udp_stat_struct->src_port = port_id;
    udp_stat_struct->dst_port = l4_stat_port;             //randomly chosen for destination app written in python
    udp_stat_struct->src_ip = port_info[port_id].start_ip_addr;
    udp_stat_struct->dst_ip = dst_ip;
    pkt_ctor(m,udp_stat_struct ,UDP_PROTO_ID);
    
    return 0;
}

void calcAverageRtt(uint64_t time_clk)
{			
	struct average_filter *average_entry = (struct average_filter *) malloc(sizeof(struct average_filter));
	float curr_pkt_rtt = (float)time_clk/(float)average_filter_len;
		
	average_entry->timestamp = curr_pkt_rtt;
	average_entry->next = NULL;
	//printf("curr_pkt_rtt %u\n", curr_pkt_rtt);
	if(average_list_size == 0)
	{
		root_rtt_average = average_entry;
		end_rtt_average = average_entry;
		average_rtt = curr_pkt_rtt;
		average_list_size++;
	}
	else if(average_list_size < average_filter_len)
	{
		average_rtt = average_rtt + curr_pkt_rtt;
		end_rtt_average->next = average_entry;
		end_rtt_average = average_entry;
		average_list_size++;
	}
	else
	{
		average_rtt = average_rtt + curr_pkt_rtt;
		end_rtt_average->next = average_entry;
		end_rtt_average = average_entry;
		struct average_filter *temp = root_rtt_average;
		average_rtt = average_rtt - root_rtt_average->timestamp;
		root_rtt_average = root_rtt_average->next;
		free(temp);
	}
}    
    
void printXfgenStats(void)
{	
	seconds_passed = seconds_passed + stats_time_period; //since each time periodic timer expires, it get updated with that seconds.
	clearScr();
	struct print_stats *curr_stats = rte_malloc ("test_stats_array" ,sizeof(struct print_stats),0);
	
	printf("-------------------------------------------------------\n");
	data_pkt_sent = global_stats.packet_transmitted;
	data_pkt_recvd = global_stats.packet_received;
	
	const uint64_t sent_pkt_per_sec		= data_pkt_sent - prev_pkt_transmitted;
	const uint64_t recvd_pkt_per_sec 	= data_pkt_recvd - prev_pkt_received;
	
	float recvd_gbps = 0;
	float sent_gbps = 0;
	
	if(l4_proto == TCP_PROTO_ID)
	{
		recvd_gbps = 8*( ((float)recvd_pkt_per_sec)*(PKT_PAYLOAD_SIZE + tcp_hdr_size + eth_hdr_size + ip_hdr_size)) / (1024*1024*1024);
		sent_gbps  = 8*( ((float)sent_pkt_per_sec)*(PKT_PAYLOAD_SIZE + tcp_hdr_size + eth_hdr_size + ip_hdr_size)) / (1024*1024*1024);
	}
	else
	{
		recvd_gbps = 8*( ((float)recvd_pkt_per_sec)*(PKT_PAYLOAD_SIZE + udp_hdr_size + eth_hdr_size + ip_hdr_size))/(1024*1024*1024);
		sent_gbps  = 8*( ((float)sent_pkt_per_sec)*(PKT_PAYLOAD_SIZE + udp_hdr_size + eth_hdr_size + ip_hdr_size))/(1024*1024*1024);		
	}
	
	curr_stats->rx_pps = recvd_pkt_per_sec;
	curr_stats->tx_pps = sent_pkt_per_sec;
	curr_stats->total_pps = recvd_pkt_per_sec + sent_pkt_per_sec ; //wrong I think what say?
	curr_stats->rx_gbps = recvd_gbps;
	curr_stats->tx_gbps = sent_gbps;
	curr_stats->total_gbps = sent_gbps + recvd_gbps;
	curr_stats->rtt = average_rtt;
	curr_stats->one_way_time = average_rtt/2;
	curr_stats->pkt_sent = data_pkt_sent;
	curr_stats->pkt_recvd = data_pkt_recvd;
	curr_stats->next = NULL;
	if(root_test_stats == NULL)
	{
		root_test_stats = curr_stats;
		end_test_stats = curr_stats;
	}
	else
	{
		end_test_stats->next = curr_stats;
		end_test_stats = curr_stats;
	}
	
	printf("TX - rate\t\t\t\t%f Gbps\n", sent_gbps);
	printf("RX - rate\t\t\t\t%f Gbps\n", recvd_gbps);
	printf("total - rate\t\t\t\t%f Gbps\n\n", recvd_gbps + sent_gbps);
	
	printf("TX - (packet per second)\t\t%lu\n", sent_pkt_per_sec);
	printf("RX - (packet per second)\t\t%lu\n", recvd_pkt_per_sec);	
	printf("total (packet per second)\t\t%lu\n\n", (sent_pkt_per_sec + recvd_pkt_per_sec));
	printf("average_rtt\t\t\t\t%f uSec\n\n", average_rtt);
	printf("total L4 packets sent\t\t\t%lu\n", data_pkt_sent);
	printf("total L4 packets received\t\t%lu\n", data_pkt_recvd);

	int pid=0;
	for (pid=0; pid<total_eth_ports; pid++) {
	
	    rte_eth_stats_get(pid, &port_stats[pid]); //array of rte_eth_stats[total_eth_ports], each element contains stats of its port.
	    printf("\n-=-=-=-=-=-=-PORTS=-=-=-=-=-=-=-=-=\n");
        printf("\npackets Received on Interface\t\t%lu\n", port_stats[pid].ipackets);
	    printf("packets transmitted from Interface\t%lu\n", port_stats[pid].opackets);
	    printf("\n-=-=-=-=-=-=-PORTS=-=-=-=-=-=-=-=-=\n");	
	
	}
	
	printf("seconds passed %u\n", seconds_passed);
	printf("test_runtime %u\n", test_runtime);

	if(pps_delay > 10 && (pps_limit < 3000000))
	{
		float change = ((float)pps_limit - sent_pkt_per_sec)/pps_limit;
		if(change != 0)
			pps_delay -= (change * pps_delay);
	}
	
	if(seconds_passed <= 5)
	{
		pps_delay = pps_delay*5;
	}
	
	if(seconds_passed >= test_runtime)
	{
		if(recvd_pkt_per_sec == 0)
		{
			writeTestStats();
			exit(0);
		}
	}
	
	
	prev_pkt_transmitted = data_pkt_sent;
	prev_pkt_received = data_pkt_recvd;	
	printf("-------------------------------------------------------\n");
}

void writeTestStats(void)
{
	unsigned count = 0;
	float aver_rtt = 0;
	while(root_test_stats != NULL)
	{
		count++;
		
		struct print_stats *temp;
		temp = root_test_stats;
		aver_rtt += temp->rtt ;			
		root_test_stats = temp->next;
		free(temp);
	}
	
	printf("\n**********    test summary     **********\n");
	printf("transmit rate = %lu \n",  pps_limit);
	printf("average rtt after test = %f\n", aver_rtt/count);
	printf("percentage Packet Loss = %f \n\n",  ((float) (data_pkt_sent - data_pkt_recvd)/data_pkt_sent) * 100 );	

	
}












































