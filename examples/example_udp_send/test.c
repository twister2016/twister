#include <stdio.h>
#include <stdint.h>
#include <tw_common.h>
#include <tw_api.h>
#include <stats.h>
#include <unistd.h>
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
/////////////////////////
int main(int, char **);
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

int main(int argc, char **argv) {
    tw_init_global(argc, argv);
    ipv4_tw0 = tw_cpu_to_be_32(tw_get_ip_addr("tw0"));
    parse_user_params("udp_traffic_data");
    tw_map_port_to_engine("tw0", "engine0");
    phy_port_id = tw_eth_name_to_id("tw0");
    tx_buf = tw_new_buffer(user_params.payload_size);
    global_stats_option.secs_passed=0;
    ppsdelay = tw_get_tsc_hz()/user_params.pps_limit;
    user_app_main(NULL);
    return 0;
}
int user_app_main(__attribute__((unused)) void * app_params) {
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
