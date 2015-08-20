#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>
#include <event_loop.h>
#include <timestamp.h>
#include <processing_engines.h>

//int PIPELINE=0;

struct user_app_parameters {
	int arg_count;
	char ** arg_vals;
};

struct user_app_parameters user_params;
struct timestamp_option * pkt_timestamp;

int main (int, char **);
int user_app_main(void *);   //__attribute__((unused))
void reply_payload(tw_udp_t *, uint16_t, tw_buf_t *, const struct tw_sockaddr_in *, uint8_t);


void reply_payload(tw_udp_t * handle, uint16_t payload_size, tw_buf_t * payload_data, const struct tw_sockaddr_in * dst_addr, uint8_t flags) {
	pkt_timestamp = (struct timestamp_option *) payload_data;
	pkt_timestamp->echo_timestamp = pkt_timestamp->timestamp;
	//udp_send(sock_fd,(void *) pkt_timestamp,sizeof(struct timestamp_option), payload_size, dst_addr->sock_ip, dst_addr->sock_port); //TODO
	tw_free(payload_data);
	return;
}

int main(int argc, char **argv) {
	tw_init_global(argc, argv);
	user_params.arg_count = argc;
	user_params.arg_vals = argv;
	tw_launch_engine(user_app_main, (void *) &user_params, USE_ALL_ENGINES); //TODO per engine modification
	return 0;
}

int user_app_main(__attribute__((unused)) void * app_params) {

	tw_udp_t server;

	int status;
	struct tw_sockaddr_in * addr;
	tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP); //TODO no time to run in standard libuv

	
	status = tw_udp_init(tw_loop, &server);
	if(status) {
		printf("Error in UDP init\n");
		exit(1);
	}
	
	addr = tw_ip4_addr("11.11.11.13", 4001);
	
	status = tw_udp_bind(&server, addr, 0);
	if(status) {
		printf("Error in UDP bind\n");
		exit(1);
	}
	
	status = tw_udp_recv_start(&server, NULL, reply_payload);
	if(status) {
		printf("Error in UDP receive start\n");
		exit(1);
	}
	tw_run(tw_loop);
	return 0;
}
