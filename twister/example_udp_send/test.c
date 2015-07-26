#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>
#include <timestamp.h>
#include <event_loop.h>

//int PIPELINE=0;

struct timestamp_option timestamp;

uint64_t curr_rate_cycle = 0, prev_rate_cycle = 0, diff_rate = 0;

int main (int, char **);
int launch_one_lcore(__attribute__((unused)) void *);
void print_payload(int, void *, int, struct sock_conn_t);

struct user_params {
	uint32_t ServerIP;
	uint16_t PayloadSize;
	uint32_t ppsLimit;
	uint32_t testRuntime;
	uint32_t StatsServerIP;
	uint16_t StatsServerPort;
};

struct user_params user_params;

void print_payload(int sock_fd, void * payload_data, int payload_size, struct sock_conn_t conn) {
	struct timestamp_option * temp_timestamp = (struct timestamp_option *) payload_data;
	uint64_t curr_time = get_current_timer_cycles();
	parse_timestamp(temp_timestamp, curr_time);
        rte_free(payload_data);
        return;
}

int parse_user_params(char * file_name) {
	uint8_t i;
	cJSON * json_file = parse_json_file(file_name);	
	if (!json_file) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}

	for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {
		cJSON * subitem = cJSON_GetArrayItem(json_file, i);
		user_params.ServerIP = convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "ServerIP")->valuestring);
		user_params.PayloadSize = convert_str_to_int(cJSON_GetObjectItem(subitem, "PayloadSize")->valuestring, 4);
		user_params.ppsLimit = convert_str_to_int(cJSON_GetObjectItem(subitem, "ppsLimit")->valuestring, 7);
		user_params.testRuntime = convert_str_to_int(cJSON_GetObjectItem(subitem, "testRuntime")->valuestring, 3);
		user_params.StatsServerIP = convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "StatsServerIP")->valuestring);
		user_params.StatsServerPort = convert_str_to_int(cJSON_GetObjectItem(subitem, "StatsServerPort")->valuestring, 4);
		global_pps_limit = user_params.ppsLimit;
	}
	return 0;
}

int main(int argc, char **argv ) {
	init_global(argc, argv);
	parse_user_params("udp_traffic_data");
	rte_eal_mp_remote_launch(launch_one_lcore, NULL, CALL_MASTER);
	return 0;
}

void send_timestamp(int sock_fd) {
	curr_rate_cycle = get_current_timer_cycles();
	diff_rate = get_time_diff(curr_rate_cycle, prev_rate_cycle, one_nsec);
	if(diff_rate >= global_pps_delay) {
		add_timestamp(&timestamp, curr_rate_cycle);
		//printf("Sending timestamp %u\n", timestamp.timestamp);
		udp_send(sock_fd,(void *)&timestamp,sizeof(struct timestamp_option), user_params.PayloadSize, user_params.ServerIP, 7777);
	}
}

int launch_one_lcore(__attribute__((unused)) void *dummy) {
	open_stats_socket(user_params.StatsServerIP, user_params.StatsServerPort); //stats pkts will be sent if port is opened
        int sockfd = udp_socket(port_info[0].start_ip_addr,7898);
	void (*rx_cb_func) (int, void *, int, struct sock_conn_t) = print_payload;
	void (*tx_cb_func) (int) = send_timestamp;
	event_flags_global = NO_FLAG_SET;
	struct event_io * io_event_rx = reg_io_event(sockfd, rx_cb_func, REPEAT_EVENT, NO_FLAG_SET, RX_CALLBACK);
	struct event_io * io_event_tx = reg_io_event(sockfd, tx_cb_func, REPEAT_EVENT, NO_FLAG_SET, TX_CALLBACK);
	start_io_events(user_params.testRuntime); //Value of 0 is for infinite loop
        return 0;
}
