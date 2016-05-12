#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <tw_common.h>
#include <tw_api.h>
#include <stats.h>
#define PacketLimit 0
#define UDP_PROTO_ID    17

uint64_t curr_time_cycle,prev_stats_calc;
uint64_t ppsdelay;

struct user_params {
    uint32_t server_ip;
    uint16_t server_port;
    uint16_t payload_size;
    uint32_t pps_limit;
    uint32_t test_runtime;
};
struct user_params user_params;
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


int main(int, char **);

uint8_t dst_port,src_port;
uint32_t dst_ip,src_ip;
struct ether_hdr * eth;
struct ipv4_hdr * ipHdr_d;
struct udp_hdr * udp_hdr_d;
uint16_t eth_type;
struct ether_addr * dst_eth;
struct iperf_test {
	char role;
	int protocol_id;
	int server_port; 
};
struct iperf_test test;

/* display usage */
void twiperf_usage(void)
{
	printf("\nUsage: twiperf ");
	printf("IPERF options:\n"
	       "  --ethernet/udp       Specify the protocol to be either ethernet or udp\n"
	       "  --server/client      Specify whether the program is a client or server\n"
	       "\n");
}

int twiperf_parse_arguments(struct iperf_test *test, int argc, char **argv)
{
    int server_flag, client_flag, udp_flag, ethernet_flag;
    static struct option longopts[] =
    {
        {"udp", no_argument, NULL, 'u'},
        {"ethernet", no_argument, NULL, 'e'},
        {"server", no_argument, NULL, 's'},
        {"client", no_argument, NULL, 'c'},
        {"port", no_argument, NULL, 'p'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };
    int flag;
    server_flag = client_flag = udp_flag = ethernet_flag = 0;
    while ((flag = getopt_long(argc, argv, "ue:cs:p:h:", longopts, NULL)) != -1) {
        switch (flag) {
            case 'c':
		client_flag = 1;
		test->role = 'c';
		break;
            case 's':
		server_flag = 1;
		test->role = 's';
		break;
            case 'e':
		ethernet_flag = 1;
		test->protocol_id = 1;
		break;
            case 'u':
		udp_flag = 1;
		test->protocol_id = 2;
		break;
            case 'p':
		test->server_port = atoi(optarg);
		printf("Port %d \n",test->server_port);
		break;
            case 'h':
		twiperf_usage();
		exit(1);
		break;
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
	return 0;

}

// UDP Server
int udp_app_server(void *);
void reply_udp_payload(tw_rx_t *, tw_buf_t *);

void reply_udp_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    eth = buffer->data;
    eth_type = tw_be_to_cpu_16(eth->ether_type);
        switch (eth_type) {
            case ETHER_TYPE_ARP:
                tw_arp_parser(buffer, "tw0");
                break;
            case ETHER_TYPE_IPv4:
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
                tw_copy_ether_addr(dst_eth, &(eth->s_addr));
                tw_send_pkt(buffer, "tw0");
                break;
                
        }
 
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
    tw_run(tw_loop);
    return 0;
}

// Ethernet Server
int udp_app_server(void *);
void reply_ether_payload(tw_rx_t *, tw_buf_t *);

void reply_ether_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_hdr * eth = buffer->data;
    tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
	tw_copy_ether_addr(dst_eth, &(eth->s_addr));
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

    tw_run(tw_loop);
    return 0;
}

// UDP Client

int user_app_main(void *);
void pkt_rx(tw_rx_t *, tw_buf_t *);
void pkt_tx(tw_tx_t *);
int parse_user_params(char *);
void pkt_rx(tw_rx_t * handle, tw_buf_t * buffer) {
    eth = buffer->data;
    if(tw_be_to_cpu_16(eth->ether_type) == ETHER_TYPE_ARP) {
        global_stats_option.pkts_rx--;
        tw_arp_parser(buffer, "tw0");
    }
    tw_free_pkt(buffer);
    return;
}

int parse_user_params(char * file_name) {
    uint8_t i;
    cJSON * json_file = tw_parse_json_file(file_name);
    if (!json_file) {
        printf("Error before: [%s]n",cJSON_GetErrorPtr());
        return -1;
    }
    for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {
        cJSON * subitem = cJSON_GetArrayItem(json_file, i);
        user_params.server_ip = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "ServerIP")->valuestring);
        user_params.server_port = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "ServerPort")->valuestring,
                                                      strlen(cJSON_GetObjectItem(subitem, "ServerPort")->valuestring));
        user_params.payload_size = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "Payload")->valuestring,
                                                      strlen(cJSON_GetObjectItem(subitem, "Payload")->valuestring));
        user_params.test_runtime = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "testRuntime")->valuestring,
                                                      strlen(cJSON_GetObjectItem(subitem, "testRuntime")->valuestring));
        user_params.pps_limit = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "ppsLimit")->valuestring, 
                                                      strlen(cJSON_GetObjectItem(subitem, "ppsLimit")->valuestring));

        global_stats_option.payload_size=user_params.payload_size ;
    }
    return 0;
}
void pkt_tx(tw_tx_t * handle)
{

curr_time_cycle = tw_get_current_timer_cycles();
if((curr_time_cycle - prev_stats_calc) > ppsdelay)
{
 prev_stats_calc=curr_time_cycle;


    if((global_stats_option.pkts_tx < PacketLimit || PacketLimit == 0) && (global_stats_option.secs_passed < user_params.test_runtime || user_params.test_runtime == 0))
    {
        if (dst_eth_addr == NULL) {
            struct arp_table * temp_arp_entry = tw_search_arp_table(tw_be_to_cpu_32(user_params.server_ip));
            if(temp_arp_entry == NULL )
            {
                if (arp_secs!=global_stats_option.secs_passed) {
                    tw_construct_arp_packet(user_params.server_ip, phy_port_id);
                    arp_secs=global_stats_option.secs_passed;
                    total_arps++;
                } else
                {
                    return;
                }
            }
            else
            dst_eth_addr = &temp_arp_entry->eth_mac;
        }
        else {
            eth = tx_buf->data;
            ip  = (struct ipv4_hdr* )(eth + 1);
            udp = (struct udp_hdr* )(ip + 1);
            udp->src_port = tw_cpu_to_be_16(7777);
            udp->dst_port = tw_cpu_to_be_16(user_params.server_port);
            udp->dgram_len = tw_cpu_to_be_16(tx_buf->size - sizeof(struct ether_hdr) - sizeof(struct ipv4_hdr));
            udp->dgram_cksum = 0;
            ip->total_length = tw_cpu_to_be_16(tx_buf->size - sizeof(struct ether_hdr));
            ip->next_proto_id = UDP_PROTO_ID;
            ip->src_addr = ipv4_tw0;
            ip->dst_addr = tw_cpu_to_be_32(user_params.server_ip);
            ip->version_ihl = 0x45;
            ip->time_to_live = 63;
            ip->hdr_checksum =tw_ipv4_cksum(ip);
            eth->ether_type = tw_cpu_to_be_16(ETHER_TYPE_IPv4);
            tw_copy_ether_addr(dst_eth_addr, &(eth->d_addr));
            tw_copy_ether_addr(port_info[phy_port_id].eth_mac, &(eth->s_addr));
            tw_send_pkt(tx_buf, "tw0");
        }
    }
 }
}

int udp_app_client(__attribute__((unused)) void * app_params) {
    tw_rx_t * rx_handle;
    tw_tx_t * tx_handle;
    tw_timer_t * timer_handle;
    int status;
    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP);
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
    tx_handle = tw_tx_init(tw_loop);
    if (tx_handle == NULL) {
        printf("Error in TX initn");
        exit(1);
    }
    status = tw_tx_start(tx_handle, pkt_tx);
    if (status) {
        printf("Error in transmit startn");
        exit(1);
    }
    tw_run(tw_loop);
    return 0;
}

// MAIN
int main(int argc, char **argv) {
    tw_init_global();
    twiperf_parse_arguments(&test, argc, argv);
    //Printing_Enable = 0; //disable the real-time printing of Tx/Rx,
    tw_map_port_to_engine("tw0", "engine0");
    dst_eth=tw_get_ether_addr("tw0");
    if(test.protocol_id == 1 && test.role == 's')
    {
	ether_app_server(NULL);
    }
    else if (test.protocol_id == 2 && test.role == 's')
    {
	udp_app_server(NULL);
    }
    else if (test.protocol_id ==1 && test.role == 'c')
    {
	printf("Functionality not supported yet\n");
    }
    else if (test.protocol_id ==2 && test.role == 'c')
    {
	udp_app_client(NULL);
    }
    return 0;
}
