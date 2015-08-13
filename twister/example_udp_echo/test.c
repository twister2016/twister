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
	tw_free(payload_data);
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
	tw_udp_t server;

	int status;
	struct tw_sockaddr_in * addr;
	tw_loop_t * event_loop = tw_default_loop();

	status = tw_udp_init(event_loop, &server);
	if(status) {
		printf("Error in UDP init\n");
		exit(1);
	}
	
	addr = tw_ip4_addr("11.11.11.13", 4001);
	
	status = tw_udp_bind(&server,addr,0);  //TODO how to give socket type and proto in libuv--using flags??
	if(status) {
                printf("Error in UDP bind\n");
                exit(1);
        }
	
	/*
	status = uv_udp_recv_start(&server, on_alloc,on_recv);
	if(status) {
                printf("Error in UDP receive start\n");
                exit(1);
        }
	
	uv_run(uv_loop);
	*/
        return 0;
}
