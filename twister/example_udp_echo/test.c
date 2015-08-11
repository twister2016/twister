#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>
#include <event_loop.h>
#include <timestamp.h>
#include <processing_engines.h>

//int PIPELINE=0;

struct timestamp_option * pkt_timestamp;

int main (int, char **);
int user_app_main(__attribute__((unused)) void *);
void reply_payload(int, void *, uint16_t, struct sock_conn_t);

void reply_payload(int sock_fd, void * payload_data, uint16_t payload_size, struct sock_conn_t conn) {
	pkt_timestamp = (struct timestamp_option *) payload_data;
	pkt_timestamp->echo_timestamp = pkt_timestamp->timestamp;
	udp_send(sock_fd,(void *) pkt_timestamp,sizeof(struct timestamp_option), payload_size, conn.dst_ip, conn.dst_port);
	rte_free(payload_data);
	return;
}

int main(int argc, char **argv )
{
	tw_init_global(argc, argv);
	tw_launch_processing_engines(user_app_main, NULL, USE_ALL_ENGINES);
	return 0;
}

int user_app_main(__attribute__((unused)) void *dummy)
{
	void (*rx_cb_func) (int, void *, uint16_t, struct sock_conn_t) = reply_payload;
	int sockfd = udp_socket(port_info[0].start_ip_addr, 7777);
	event_flags_global = NO_FLAG_SET;
	struct event_io * io_event_rx = reg_io_event(sockfd, rx_cb_func, REPEAT_EVENT, event_flags_global, RX_CALLBACK);
	start_io_events(INFINITE_LOOP);
        return 0;
}