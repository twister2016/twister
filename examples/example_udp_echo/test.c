#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>
#include <eth.h>
#include <event_loop.h>
#include <timestamp.h>
#include <processing_engines.h>

struct user_app_parameters {
    int arg_count;
    char ** arg_vals;
};

struct user_app_parameters user_params;
struct timestamp_option * pkt_timestamp;

int main(int, char **);
int user_app_main(void *);
void reply_payload(tw_rx_t *, tw_buf_t *);

void reply_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_hdr * eth = buffer->data;
    if (tw_match_port_ether_addr(&(eth->d_addr), "tw0") || is_broadcast_ether_addr(&(eth->d_addr))) {
        switch (tw_be_to_cpu_16(eth->ether_type)) {
            case ETHER_TYPE_ARP:
                tw_arp_parser(buffer, "tw0");
                break;
            default:
                tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
                tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
                tw_send_pkt(buffer, "tw0");
                break;
        }
    } else
        tw_free_pkt(buffer);
    return;
}

int main(int argc, char **argv) {
    tw_init_global(argc, argv);
    user_params.arg_count = argc;
    user_params.arg_vals = argv;
    tw_map_port_to_engine("tw0", "engine0");
    tw_launch_engine(user_app_main, (void *) &user_params, USE_ALL_ENGINES);

    return 0;
}

int user_app_main(__attribute__((unused)) void * app_params) {

    tw_rx_t * server;
    int status;
    struct tw_sockaddr_in * addr;
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
