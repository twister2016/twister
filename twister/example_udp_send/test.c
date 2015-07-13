#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>
#include <timestamp.h>
#include <event_loop.h>

//int PIPELINE=0;

struct timestamp_option timestamp;

int main (int, char **);
int launch_one_lcore(__attribute__((unused)) void *);

void print_payload(void * payload_data, int payload_size, struct sock_conn_t conn) {
        printf("CB Data %s, Data Length %d\n", (char *) payload_data, payload_size);
        rte_free(payload_data);
        return;
}

int main(int argc, char **argv ) {
	init_global(argc, argv);
	rte_eal_mp_remote_launch(launch_one_lcore, NULL, CALL_MASTER);
	return 0;
}

void send_timestamp(int sock_fd) {
	add_timestamp(&timestamp);
	udp_send(sock_fd,(void *)&timestamp,sizeof(struct timestamp_option),convert_ip_str_to_dec("11.11.7.166"),8787);
}

int launch_one_lcore(__attribute__((unused)) void *dummy) {
        int sockfd = udp_socket(port_info[0].start_ip_addr,7898);
	void (*rx_cb_func) (void *, int, struct sock_conn_t) = print_payload;
	void (*tx_cb_func) (int) = send_timestamp;
	event_flags_global = NO_FLAG_SET;
	struct event_io * io_event_rx = reg_io_event(sockfd, rx_cb_func, REPEAT_EVENT, NO_FLAG_SET, RX_CALLBACK);
	struct event_io * io_event_tx = reg_io_event(sockfd, tx_cb_func, REPEAT_EVENT, NO_FLAG_SET, TX_CALLBACK);
	start_io_events();
        return 0;
}
