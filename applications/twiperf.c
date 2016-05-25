#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <tw_common.h>
#include <tw_api.h>
#include <stats.h>
#include "twiperf.h"
#define PacketLimit 0
#define UDP_PROTO_ID    17

uint64_t curr_time_cycle,prev_stats_calc;
uint64_t ppsdelay;

////////////////////////
uint64_t arp_secs=-1;
struct ether_hdr * eth;
struct ether_addr * stats_eth_addr;
struct ipv4_hdr * ip;
struct udp_hdr * udp;
int phy_port_id;
uint32_t total_arps;
static struct ether_addr * dst_eth_addr;
uint32_t ipv4_tw0;
tw_buf_t * tx_buf;
tw_buf_t * tx_buf_stats;
tw_loop_t * tw_loop;                                                                                    

int i_errno;
int main(int, char **);

uint8_t dst_port,src_port;
uint32_t dst_ip,src_ip;
struct ether_hdr * eth;
struct ipv4_hdr * ipHdr_d;
struct udp_hdr * udp_hdr_d;
uint16_t eth_type;
struct ether_addr * dst_eth;
struct iperf_test test;
struct iperf_stats test_stats;

void sig_handler(int signo)
{
    if (signo == SIGINT){
        twiprintf(&test, summary_dot_line);
        twiprintf(&test, summary_head);
        twiprintf(&test, summary_stats_number, 0.0, test_stats.interval_window, test_stats.total_transfered_bytes, test_stats.bandwidth, test_stats.datagrams_sent-test_stats.datagrams_recv, test_stats.datagrams_sent);
        exit(1);
  }
}



int twiperf_parse_arguments(struct iperf_test *test, int argc, char **argv)
{

    int server_flag, client_flag, udp_flag, ethernet_flag;
    static struct option longopts[] =
    {
        {"udp", no_argument, NULL, 'u'},
        {"ethernet", no_argument, NULL, 'e'},
        {"server", no_argument, NULL, 's'},
        {"client", required_argument, NULL, 'c'},
        {"help", no_argument, NULL, 'h'},
        {"port", required_argument, NULL, 'p'},
        {"bytes", required_argument, NULL, 'n'},
        {NULL, 0, NULL, 0}
    };
    int flag;
    server_flag = client_flag = udp_flag = ethernet_flag = 0;
    test->packet_size = 64; //initialized to default value of 64 bytes pcket size
    test->server_port = 5001;  //initialized to default port of 5001
    test->client_port = 7777;  //initialized to default port of 5001
    test->test_runtime = 0;  //initialized to default infinite runtime
//    test->client_mac = tw_get_ether_addr("tw0");

	udp_flag = 1;
        test->protocol_id = 2;

    while ((flag = getopt_long(argc, argv, "n:p:ue:cs:h:", longopts, NULL)) != -1) {
        switch (flag) {
            case 'p':
                test->server_port = atoi(optarg);
                break;
            case 'n':
                test->packet_size =atoi(optarg);
                break;
            case 'c':
                client_flag = 1;
                test->role = 2;
                test->client_mac = tw_get_ether_addr("tw0");
                //test->server_ip =tw_cpu_to_be_32( tw_convert_ip_str_to_dec(strdup(optarg)));
                test->server_ip =tw_convert_ip_str_to_dec(strdup(optarg));
                test->client_ip = tw_cpu_to_be_32(tw_get_ip_addr("tw0"));
                break;
            case 's':
                server_flag = 1;
                test->role = 1;
                test->server_mac = tw_get_ether_addr("tw0");
                test->server_ip = tw_cpu_to_be_32(tw_get_ip_addr("tw0"));
                break;
            case 'e':
                ethernet_flag = 1;
		udp_flag = 0;
                test->protocol_id = 1;
                break;
            case 'u':
                udp_flag = 1;
                test->protocol_id = 2;
                break;
            case 'h':
                twiperf_usage();
                exit(1);
                break;
            default:
                twiperf_usage();
                exit(1);
        }
	}
	if(server_flag == 1 && client_flag == 1 || udp_flag == 1 && ethernet_flag == 1 )
	{
		twiperf_usage();
        exit(1);
	}
	if(server_flag == 0 && client_flag == 0 || udp_flag == 0 && ethernet_flag == 0 )
	{
		twiperf_usage();
        exit(1);
	}
    if ((test->role != 1) && (test->role != 2)) {
        i_errno = IENOROLE;
        return -1;
    }
    if (udp_flag == 1 && test->packet_size > MAX_UDP_BLOCKSIZE) {
        i_errno = IEUDPBLOCKSIZE;
        return -1;
    }
	return 0;

}

void print_perf_stats();

// UDP Server
int udp_app_server(void *);
void reply_udp_payload(tw_rx_t *, tw_buf_t *);
void reply_udp_payload(tw_rx_t * handle, tw_buf_t * buffer) {
     eth = buffer->data;
     eth_type = tw_be_to_cpu_16(eth->ether_type);
     ipHdr_d = buffer->data + sizeof(struct ether_hdr);
     dst_ip  = (ipHdr_d->dst_addr);
     src_ip = (ipHdr_d->src_addr);
     ipHdr_d->dst_addr = (src_ip);
     ipHdr_d->src_addr = (dst_ip);
     ipHdr_d->hdr_checksum = tw_ipv4_cksum(ipHdr_d);
     udp_hdr_d = (struct udp_hdr *)ipHdr_d + sizeof(struct ipv4_hdr);
     dst_port = (udp_hdr_d->dst_port);
     src_port = (udp_hdr_d->src_port);
     udp_hdr_d->dst_port = (src_port);
     udp_hdr_d->src_port = (dst_port);
     udp_hdr_d->dgram_cksum = 0;
     tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
     tw_copy_ether_addr(test.server_mac, &(eth->s_addr));
     tw_send_pkt(buffer, "tw0");
                 
}

int udp_app_server(__attribute__((unused)) void * app_params) {

    tw_rx_t * server;
    int status;
    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP);

    server = tw_rx_init(tw_loop);
    if (server == NULL) {
        printf("Error in RX init\n");
        exit(1);
    }

    status = tw_rx_start(server, reply_udp_payload);
    if (status) {
        printf("Error in receive start\n");
        exit(1);
    }
    tw_timer_t * timer_handle;
    timer_handle = tw_timer_init(tw_loop);
    tw_timer_start(timer_handle, print_perf_stats, 1000);
    twiprintf(&test, stats_head);
    tw_run(tw_loop);
    return 0;
}

// Ethernet Server
int udp_app_server(void *);
void reply_ether_payload(tw_rx_t *, tw_buf_t *);

void reply_ether_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_hdr * eth = buffer->data;
    tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
	tw_copy_ether_addr(test.server_mac, &(eth->s_addr));
	tw_send_pkt(buffer, "tw0");
}

int ether_app_server(__attribute__((unused)) void * app_params) {

    tw_rx_t * server;
    int status;
    struct tw_sockaddr_in * addr;
    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP);

    server = tw_rx_init(tw_loop);
    if (server == NULL) {
        printf("Error in RX init\n");
        exit(1);
    }

    status = tw_rx_start(server, reply_ether_payload);
    if (status) {
        printf("Error in receive start\n");
        exit(1);
    }
    tw_timer_t * timer_handle;
    timer_handle = tw_timer_init(tw_loop);
    tw_timer_start(timer_handle, print_perf_stats, 1000);
    twiprintf(&test, stats_head);
    tw_run(tw_loop);
    return 0;
}

// UDP j Client

int user_app_main(void *);
void pkt_rx(tw_rx_t *, tw_buf_t *);
void pkt_tx(tw_tx_t *);
void tw_udp_connect();

void pkt_rx(tw_rx_t * handle, tw_buf_t * buffer) {
    eth = buffer->data;
    test_stats.datagrams_recv++;
    if(tw_be_to_cpu_16(eth->ether_type) == ETHER_TYPE_ARP) {
        tw_stats.pkts_rx--;
        tw_arp_parser(buffer, "tw0");
    }
    tw_free_pkt(buffer);
    return;
}
void print_perf_stats(){

    //tw_calc_global_stats();
    test_stats.interval_window = test_stats.interval_window + 1.0;
    uint64_t bytes = (   (  (tw_stats.rx_pps + tw_stats.tx_pps )*(test.packet_size)  ) /1000   ); //KBytes
    float bandwidth = (bytes *8*1000 )/ (float)(1048576.0) ; // Mbit / sec 
    twiprintf(&test, stats_number, test_stats.interval_window-1.0, test_stats.interval_window, tw_stats.rx_pps,tw_stats.tx_pps, bytes, bandwidth, test_stats.datagrams_sent, test_stats.datagrams_recv);

    test_stats.total_transfered_bytes += bytes;
    test_stats.bandwidth += bandwidth;
}

void pkt_tx(tw_tx_t * handle)
{

    curr_time_cycle = tw_get_current_timer_cycles();
    if((curr_time_cycle - prev_stats_calc) > ppsdelay)
    {
        prev_stats_calc=curr_time_cycle;
        if((tw_stats.pkts_tx < PacketLimit || PacketLimit == 0) && (tw_stats.secs_passed < test.test_runtime || test.test_runtime == 0))
        {
            // Now arp is resolved, lets send a udp packet,
            test.eth = test.tx_buf->data;
            test.ip  = (test.tx_buf->data + sizeof(struct ether_hdr));
            test.udp = test.tx_buf->data + sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr);
            test.udp->src_port = tw_cpu_to_be_16(test.client_port);
            test.udp->dst_port = tw_cpu_to_be_16(test.server_port);
            test.udp->dgram_len = tw_cpu_to_be_16(test.tx_buf->size - sizeof(struct ether_hdr) - sizeof(struct ipv4_hdr));
            test.udp->dgram_cksum = 0;
            test.ip->total_length = tw_cpu_to_be_16(test.tx_buf->size - sizeof(struct ether_hdr));
            test.ip->next_proto_id = UDP_PROTO_ID;
            test.ip->src_addr = test.client_ip;
            test.ip->dst_addr = tw_cpu_to_be_32(test.server_ip);
            test.ip->version_ihl = 0x45;
            test.ip->time_to_live = 63;
            test.ip->hdr_checksum =tw_ipv4_cksum(test.ip);
            test.eth->ether_type = tw_cpu_to_be_16(ETHER_TYPE_IPv4);
            tw_copy_ether_addr(test.server_mac, &(test.eth->d_addr));
            tw_copy_ether_addr(test.client_mac, &(test.eth->s_addr));
            tw_send_pkt(test.tx_buf, "tw0");
            test_stats.datagrams_sent++;
            //usleep(1000000);
        }
    }
}

int udp_app_client(__attribute__((unused)) void * app_params) {
    tw_rx_t * rx_handle;
    
    tw_timer_t * timer_handle;
    int status;
    tw_loop = tw_default_loop(INFINITE_LOOP);
    rx_handle = tw_rx_init(tw_loop);
    if (rx_handle == NULL) {
        printf("Error in RX initn");
        exit(1);
    }
    status = tw_rx_start(rx_handle, pkt_rx);
    if (status) {
        printf("Error in receive startn");
        exit(1);
    }
    timer_handle = tw_timer_init(tw_loop);
    tw_timer_start(timer_handle, tw_udp_connect, 1000);
    tw_run(tw_loop);
    return 0;
}


void tw_udp_connect(){
    static arpCount=0;
    struct arp_table* temp_arp_entry=NULL;
    if (arpCount < 4){
        temp_arp_entry = tw_search_arp_table(tw_be_to_cpu_32(test.server_ip));
        if(temp_arp_entry == NULL )  {
             tw_construct_arp_packet(test.server_ip, phy_port_id);                                                                    
             arpCount++;
             return;
         }
        else {

             test.server_mac = &temp_arp_entry->eth_mac;
             struct in_addr server_ip; server_ip.s_addr = tw_cpu_to_be_32(test.server_ip);
             struct in_addr client_ip; client_ip.s_addr = (test.client_ip);
             printf("local %s, port %u ", inet_ntoa(client_ip), test.client_port);
             printf("connected to %s port %u\n", inet_ntoa(server_ip), test.server_port);
                /////////////////////////////////////////////
             tw_loop->timer_handle_queue=NULL;
             tw_loop->active_handles--;
             tw_tx_t * tx_handle = tw_tx_init(tw_loop);
             if (tx_handle == NULL) {
                 printf("Error in TX initn");
                 exit(1);
             }
             int status = tw_tx_start(tx_handle, pkt_tx);
             if (status) {
                  printf("Error in transmit startn");
                  exit(1);
             }
             tw_timer_t * timer_handle;
             timer_handle = tw_timer_init(tw_loop);
             tw_timer_start(timer_handle, print_perf_stats, 1000);
             twiprintf(&test, stats_head);
             //////////////////////////////////////////////
        }
    }
    else if (arpCount>=4 && temp_arp_entry == NULL ){
        printf("unable to find the ip on network.\n");
        exit(1);
    }

}

// MAIN
int main(int argc, char **argv) {
    tw_init_global();
    if (twiperf_parse_arguments(&test, argc, argv) < 0) {
        iperf_err(&test, "parameter error - %s", iperf_strerror(i_errno));
        fprintf(stderr, "\n");
        twiperf_usage();
        exit(1); 
    }
    
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    
    Printing_Enable = 1; //disable the real-time printing of Tx/Rx,
    tw_map_port_to_engine("tw0", "engine0");
    if(test.protocol_id == 1 && test.role == 1)
    {
	    ether_app_server(NULL);
    }
    else if (test.protocol_id == 2 && test.role == 1)
    {
        udp_app_server(NULL);
    }
    else if (test.protocol_id == 1 && test.role == 2)
    {
        printf("Functionality not supported yet\n");
    }
    if (test.protocol_id == 2 && test.role == 2)
	{
	//parse_user_params("udp_traffic_data");
	test.tx_buf = tw_new_buffer(test.packet_size);
	tw_stats.secs_passed=0;
    struct in_addr server_ip; server_ip.s_addr = tw_cpu_to_be_32(test.server_ip);
    twiprintf(&test, on_host_conn, inet_ntoa(server_ip), test.server_port);
    udp_app_client(NULL);
	}
    
    return 0;
}

    




