#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>
#include <timestamp.h>
#include <event_loop.h>
#include <processing_engines.h>

struct user_app_parameters {
    int arg_count;
    char ** arg_vals;
};

struct user_app_parameters user_app_params;
struct timestamp_option timestamp;

uint64_t curr_rate_cycle = 0, prev_rate_cycle = 0, diff_rate = 0;

uint64_t prev_arp_send_time = 0;

int main(int, char **);
int user_app_main(__attribute__((unused)) void *);
void print_payload(tw_rx_t *, int, tw_buf_t *);
void send_pkt(tw_tx_t * tx_handle);

struct user_params {
    char * ServerIP;
    uint16_t PayloadSize;
    uint32_t ppsLimit;
    uint32_t testRuntime;
    char * StatsServerIP;
    uint16_t StatsServerPort;
    uint32_t tagHeatIP;
};

struct user_params user_params;

int parse_user_params(char * file_name) {
    uint8_t i;
    cJSON * json_file = parse_json_file(file_name);
    if (!json_file) {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        return -1;
    }

    for (i = 0; i < cJSON_GetArraySize(json_file); i++) {
        cJSON * subitem = cJSON_GetArrayItem(json_file, i);
        user_params.ServerIP = cJSON_GetObjectItem(subitem, "ServerIP")->valuestring;
        user_params.PayloadSize = convert_str_to_int(cJSON_GetObjectItem(subitem, "PayloadSize")->valuestring, 4);
        user_params.ppsLimit = convert_str_to_int(cJSON_GetObjectItem(subitem, "ppsLimit")->valuestring, 7);
        user_params.testRuntime = convert_str_to_int(cJSON_GetObjectItem(subitem, "testRuntime")->valuestring, 3);
        user_params.StatsServerIP = cJSON_GetObjectItem(subitem, "StatsServerIP")->valuestring;
        user_params.StatsServerPort = convert_str_to_int(cJSON_GetObjectItem(subitem, "StatsServerPort")->valuestring, 4);
        user_params.tagHeatIP = convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "vm_ip")->valuestring);
        global_stats_option[tw_current_engine()].tag_heat_ip = user_params.tagHeatIP;
        global_pps_limit[tw_current_engine()] = user_params.ppsLimit;
    }
    return 0;
}

void parse_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_hdr * eth = buffer->data;
    if (tw_match_port_ether_addr(&(eth->d_addr), buffer->port_name)) {
        switch (tw_be_to_cpu_16(eth->ether_type)) {
            case ETHER_TYPE_ARP:
                tw_arp_parser(buffer, buffer->port_name);
                break;
            default:
                tw_free_pkt(buffer);
                break;
        }
    } else
        tw_free_pkt(buffer);
    return;
}

void send_pkt(tw_tx_t * tx_handle) {
    struct ether_addr * dst_eth_addr = tw_search_arp_table(user_params.ServerIP);
    if (dst_eth_addr == NULL) {
        if (tw_if_timeout_millisec(tx_handle->last_run_time, prev_arp_send_time, 100)) {
            tw_send_arp_request(user_params.ServerIP, "tw0");
            prev_arp_send_time = tx_handle->last_run_time;
        }
    } else {
        tw_buf_t * buffer = tw_new_buffer(user_params.PayloadSize);
        struct ether_hdr * eth = buffer->data;
        tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
        tw_copy_ether_addr(dst_eth_addr, &(eth->d_addr));
        eth->ether_type = tw_cpu_to_be_16(ETHER_TYPE_IPv4);
        tw_send_pkt(buffer, "tw0");
        tw_free((void *) buffer);
    }
    return;
}

int main(int argc, char **argv) {
    tw_init_global(argc, argv);
    parse_user_params("udp_traffic_data");
    user_app_params.arg_count = argc;
    user_app_params.arg_vals = argv;
    tw_map_port_to_engine("tw0", "engine0");
    tw_launch_engine(user_app_main, (void *) &user_app_params, USE_ALL_ENGINES); //TODO per engine modification
    return 0;
}

int user_app_main(__attribute__((unused)) void * app_params) {

    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP); //TODO no time_to_run in standard libuv

    uint64_t timeout = 1000;
    tw_tx_t * tx_handle;
    tw_timer_t * timer_handle;
    tw_rx_t * rx_handle;
    int status;

    struct tw_sockaddr_in * client_addr;

    rx_handle = tw_rx_init(tw_loop);
    
    status = tw_rx_start(rx_handle, parse_payload);
    if (status) {
        printf("Error in UDP receive start\n");
        exit(1);
    }

    tx_handle = tw_tx_init(tw_loop);

    status = tw_tx_start(tx_handle, send_pkt);

    if (status) {
        printf("Error in TX start\n");
        exit(1);
    }
    tw_run(tw_loop);
    return 0;
}
