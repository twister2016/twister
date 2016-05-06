#include <stdio.h>
#include <stdint.h>
#include <tw_api.h>
#include <tw_common.h>


int main(int, char **);
int user_app_main(void *);
struct ether_addr * dst_eth0;
struct ether_addr * dst_eth1;
void reply_payload(tw_rx_t *, tw_buf_t *);

void reply_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_hdr * eth = buffer->data;
    tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
    tw_copy_ether_addr(dst_eth0, &(eth->s_addr));
    tw_send_pkt(buffer, buffer->port_name);
}

int main(int argc, char **argv) {
    tw_init_global(argc, argv);
    tw_map_port_to_engine("tw0", "engine0");
    tw_map_port_to_engine("tw1", "engine0");
    dst_eth0=tw_get_ether_addr("tw0");
    dst_eth1=tw_get_ether_addr("tw1");
    user_app_main(NULL);
    return 0;
}

int user_app_main(__attribute__((unused)) void * app_params) {

    tw_rx_t * server;
    int status;
    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP);

    server = tw_rx_init(tw_loop);
    if (server == NULL) {
        printf("Error in RX init\n");
        exit(1);
    }
    status = tw_rx_start(server, reply_payload);
    if (status) {
        printf("Error in receive start\n");
        exit(1);
    }

    tw_run(tw_loop);
    return 0;
}
