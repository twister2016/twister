#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>
#include <event_loop.h>

//int PIPELINE=0;

int main (int, char **);
int launch_one_lcore(__attribute__((unused)) void *);
void print_payload(void *, int, struct sock_conn_t);

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

int launch_one_lcore(__attribute__((unused)) void *dummy) {
	
	void (*rx_cb_func) (void *, int, struct sock_conn_t) = print_payload;

	int sockfd = udp_socket(port_info[0].start_ip_addr, 8787);
	struct event_io * io_event = reg_io_event(sockfd, rx_cb_func, 1, NO_FLAG_SET);
	start_io_events();
        return 0;
}

