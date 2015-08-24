#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>
#include <timestamp.h>
#include <event_loop.h>
#include <processing_engines.h>

//int PIPELINE=0;

struct user_app_parameters {
	int arg_count;
	char ** arg_vals;
};

struct user_app_parameters user_app_params;
struct timestamp_option timestamp;

uint64_t curr_rate_cycle = 0, prev_rate_cycle = 0, diff_rate = 0;

int main (int, char **);
int user_app_main(__attribute__((unused)) void *);
void print_payload(tw_udp_t *, int, tw_buf_t *, struct tw_sockaddr_in *, uint8_t);
void send_timestamp(tw_tx_t * tx_handle);

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
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}

	for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {
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

void parse_payload(tw_udp_t * handle, int payload_size, tw_buf_t * buffer, struct tw_sockaddr_in * dst_addr, uint8_t flags) {
	struct timestamp_option * temp_timestamp = (struct timestamp_option *) buffer->data;
	uint64_t curr_time = tw_get_current_timer_cycles();
	parse_timestamp(temp_timestamp, curr_time);
    tw_free_buffer(buffer);
    return;
}

void send_timestamp(tw_tx_t * tx_handle) {
	tw_buf_t * buffer = tw_new_buffer(sizeof(struct timestamp_option));
	struct timestamp_option * temp_timestamp = (struct timestamp_option *) buffer->data;
	tw_add_timestamp(temp_timestamp);
	tw_udp_send(tx_handle->sock_fd, buffer, sizeof(struct timestamp_option), 0, tx_handle->dst_addr);
	return;
}

void send_stats(tw_timer_t * timer_handle) {
	tw_buf_t * buffer = tw_new_buffer(sizeof(struct stats_option));
	tw_memcpy(buffer->data, (void *) &global_stats_option[tw_current_engine()], sizeof(struct stats_option));
	tw_udp_send(timer_handle->sock_fd, buffer, sizeof(struct stats_option), 0, timer_handle->dst_addr);
	return;
}

int main(int argc, char **argv ) {
	tw_init_global(argc, argv);
	parse_user_params("udp_traffic_data");
	user_app_params.arg_count = argc;
	user_app_params.arg_vals = argv;
	tw_launch_engine(user_app_main, (void *) &user_app_params, USE_ALL_ENGINES); //TODO per engine modification
	return 0;
}

int user_app_main(__attribute__((unused)) void * app_params) {

	tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP); //TODO no time_to_run in standard libuv
	
	
	uint64_t timeout = 1000;
	tw_tx_t * tx_handle;
	tw_timer_t * timer_handle;
	tw_udp_t * client;
	int status;
	
	struct tw_sockaddr_in * client_addr;
	
	client = tw_udp_init(tw_loop);
	/*
	if(status) {
		printf("Error in UDP init\n");
		exit(1);
	}*/

	client_addr = tw_ip4_addr("34.34.34.11", 7777);   //TODO add tw0 logic
	
	status = tw_udp_bind(client, client_addr, 0);
	if(status) {
		printf("Error in UDP bind\n");
		exit(1);
	}
	status = tw_udp_recv_start(client, NULL, parse_payload);
	if(status) {
		printf("Error in UDP receive start\n");
		exit(1);
	}
	
	struct tw_sockaddr_in * stats_addr;
	stats_addr = tw_ip4_addr(user_params.StatsServerIP, user_params.StatsServerPort);
	
	timer_handle = tw_timer_init(tw_loop);
	/*
	if(status) {
		printf("Error in timer init\n");
		exit(1);
	}*/
	
	tw_timer_bind(timer_handle, stats_addr, client->sock_fd, 0);

	status = tw_timer_start(timer_handle, send_stats, timeout, 1);   //timeout in millisecs
	
	struct tw_sockaddr_in * server_addr;
	server_addr = tw_ip4_addr(user_params.ServerIP, 7777);
	
	tx_handle = tw_udp_tx_init(tw_loop);
	/*
	if(status) {
		printf("Error in TX callback init\n");
		exit(1);
	}*/
	
	status = tw_udp_tx_bind(tx_handle, server_addr, client->sock_fd, 0);
	if(status) {
		printf("Error in TX bind\n");
		exit(1);
	}

	status = tw_udp_tx_start(tx_handle, send_timestamp);

	if(status) {
		printf("Error in TX start\n");
		exit(1);
	}
	
	tw_run(tw_loop);
	return 0;
}
