#include <stdio.h>
#include <stdint.h>
#include <tw_common.h>
#include <tw_api.h>
#include <stats.h>


struct user_app_parameters {
    int arg_count;
    char ** arg_vals;
};

#define UDP_PROTO_ID	17

struct user_params {
    uint32_t server_ip;
    uint16_t server_port;
    uint16_t payload_size;
    uint32_t pps_limit;
    uint32_t test_runtime;
    uint32_t stats_server_ip;
    uint16_t stats_server_port;
    uint32_t tag_heat_ip;
};

struct user_params user_params;
struct user_app_parameters user_args;
struct timestamp_option * pkt_timestamp;

int main(int, char **);
int user_app_main(void *);
void pkt_rx(tw_rx_t *, tw_buf_t *);
void pkt_tx(tw_tx_t *);
int parse_user_params(char *);

void pkt_rx(tw_rx_t * handle, tw_buf_t * buffer) {
    tw_free_pkt(buffer);
    return;
}

int parse_user_params(char * file_name) {
	uint8_t i;
	cJSON * json_file = tw_parse_json_file(file_name);	
	if (!json_file) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}

	for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {
		cJSON * subitem = cJSON_GetArrayItem(json_file, i);
		user_params.server_ip = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "ServerIP")->valuestring);
		user_params.server_port = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "ServerPort")->valuestring, 4);
		user_params.payload_size = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "Payload")->valuestring, 4);
		//user_params.pps_limit = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "ppsLimit")->valuestring, 7);
		user_params.test_runtime = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "testRuntime")->valuestring, 3);
		user_params.stats_server_ip = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "StatsServerIP")->valuestring);
		user_params.stats_server_port = tw_convert_str_to_int(cJSON_GetObjectItem(subitem, "StatsServerPort")->valuestring, 4);
		//user_params.tag_heat_ip = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "vm_ip")->valuestring);
		//global_stats_option.tag_heat_ip = user_params.tag_heat_ip;
		//global_pps_limit = user_params.pps_limit;
	}
	return 0;
}

void pkt_tx(tw_tx_t * handle) {
    int phy_port_id = tw_eth_name_to_id("tw0");
    if(phy_port_id < 0) 
        return;

    static struct ether_addr * dst_eth_addr;
    if (dst_eth_addr == NULL) {
        struct arp_table * temp_arp_entry = tw_search_arp_table(user_params.server_ip);
        if(temp_arp_entry == NULL) {
            tw_construct_arp_packet(user_params.server_ip, phy_port_id);
            tw_sleep_msec(10);
        }
        else
            dst_eth_addr = &temp_arp_entry->eth_mac;
		
    }
    else {
        tw_buf_t * tx_buf = tw_new_buffer(user_params.payload_size);
        struct ether_hdr * eth = tx_buf->data;
        struct ipv4_hdr * ip  = eth + 1;
        struct udp_hdr * udp = ip + 1;
        udp->src_port = tw_cpu_to_be_16(7777);
        udp->dst_port = tw_cpu_to_be_16(user_params.stats_server_port);
        udp->dgram_len = tw_cpu_to_be_16(tx_buf->size - sizeof(struct ether_hdr) - sizeof(struct ipv4_hdr));
        //udp_hdr_d->dgram_cksum = udp_hdr_d->src_port + udp_hdr_d->dst_port + pkt->pkt_len;
        udp->dgram_cksum = 0;

        ip->total_length = tw_cpu_to_be_16(tx_buf->size - sizeof(struct ether_hdr));
        ip->next_proto_id = UDP_PROTO_ID;
        ip->src_addr = tw_cpu_to_be_32(tw_get_ip_addr("tw0"));
        ip->dst_addr = tw_cpu_to_be_32(user_params.server_ip);
        ip->version_ihl = 0x45;
        ip->time_to_live = 63;
        ip->hdr_checksum = 0;
        ip->hdr_checksum =rte_ipv4_cksum(ip);

        tw_copy_ether_addr(dst_eth_addr, &(eth->d_addr));
        tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
        tw_send_pkt(tx_buf, "tw0");
    }
}

void send_stats(tw_timer_t * timer_handle) {
    int phy_port_id = tw_eth_name_to_id("tw0");
    if(phy_port_id < 0)
        return;

    static struct ether_addr * stats_eth_addr;
    if (stats_eth_addr == NULL) {
        struct arp_table * temp_arp_entry = tw_search_arp_table(user_params.stats_server_ip);
        if(temp_arp_entry == NULL) {
            tw_construct_arp_packet(user_params.stats_server_ip, phy_port_id);
        }
        else
            stats_eth_addr = &(temp_arp_entry->eth_mac);
    }
    else {
        tw_buf_t * stats_buf = tw_new_buffer(sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr) \
                                        + sizeof(struct udp_hdr) + sizeof(struct stats_option*));
        struct ether_hdr * eth = stats_buf->data;
        struct ipv4_hdr * ip  = eth + 1;
        struct udp_hdr * udp = ip + 1;
        struct stats_option * stats_to_send = udp + 1;
        
        tw_memcpy(stats_to_send, (void const *) &global_stats_option[0], sizeof(stats_to_send)); //TODO remove '0'

        udp->src_port = tw_cpu_to_be_16(7777);
        udp->dst_port = tw_cpu_to_be_16(user_params.stats_server_port);
        udp->dgram_len = tw_cpu_to_be_16(stats_buf->size - sizeof(struct ether_hdr) - sizeof(struct ipv4_hdr));
        //udp_hdr_d->dgram_cksum = udp_hdr_d->src_port + udp_hdr_d->dst_port + pkt->pkt_len;
        udp->dgram_cksum = 0;

        ip->total_length = tw_cpu_to_be_16(stats_buf->size - sizeof(struct ether_hdr));
        ip->next_proto_id = UDP_PROTO_ID;
        ip->src_addr = tw_cpu_to_be_32(tw_get_ip_addr("tw0"));
        ip->dst_addr = tw_cpu_to_be_32(user_params.stats_server_ip);
        ip->version_ihl = 0x45;
        ip->time_to_live = 63;
        ip->hdr_checksum = 0;
        ip->hdr_checksum =rte_ipv4_cksum(ip); //TODO write a wrapper func for rte_ipv4_cksum

        tw_copy_ether_addr(stats_eth_addr, &(eth->d_addr));
        tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
        tw_send_pkt(stats_buf, "tw0");
    }

}

int main(int argc, char **argv) {
    tw_init_global(argc, argv);
    user_args.arg_count = argc;
    user_args.arg_vals = argv;
    parse_user_params("udp_traffic_data");
    tw_map_port_to_engine("tw0", "engine0");
    tw_launch_engine(user_app_main, (void *) &user_args, USE_ALL_ENGINES);

    return 0;
}

int user_app_main(__attribute__((unused)) void * app_params) {

    tw_rx_t * rx_handle;
    tw_tx_t * tx_handle;
    tw_timer_t * timer_handle;
    int status;
    struct tw_sockaddr_in * addr;
    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP);

    rx_handle = tw_rx_init(tw_loop);
    if (rx_handle == NULL) {
        printf("Error in RX init\n");
        exit(1);
    }
    status = tw_rx_start(rx_handle, pkt_rx);
    if (status) {
        printf("Error in receive start\n");
        exit(1);
    }

    tx_handle = tw_tx_init(tw_loop);
    if (tx_handle == NULL) {
        printf("Error in RX init\n");
        exit(1);
    }
    status = tw_tx_start(tx_handle, pkt_tx);
    if (status) {
        printf("Error in receive start\n");
        exit(1);
    }
    
    timer_handle = tw_timer_init(tw_loop);
    tw_timer_bind(timer_handle, NULL, 0, 0);
    tw_timer_start(timer_handle, send_stats, 1, 1);

    tw_run(tw_loop);
    return 0;
}
