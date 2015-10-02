#include <stdio.h>
#include <stdint.h>
#include <tw_common.h>
#include <tw_api.h>

struct user_app_parameters {
    int arg_count;
    char ** arg_vals;
};
//int timer_callback_eth(); move to send
struct user_app_parameters user_params;
struct timestamp_option * pkt_timestamp;

int main(int, char **);
int user_app_main(void *);
void reply_payload(tw_rx_t *, tw_buf_t *);

/*int timer_callback_eth(){
     struct ether_hdr * eth = buffer->data; //initialize buffer from memeory
    tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
    tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
    tw_send_pkt(buffer, "tw0");
}*/
void reply_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_hdr * eth = buffer->data;
    struct ipv4_hdr * ipHdr_d;
    struct udp_hdr * udp_hdr_d;
    uint16_t eth_type = tw_be_to_cpu_16(eth->ether_type);
    if (eth_type == ETHER_TYPE_VLAN)
        rte_vlan_strip(buffer->pkt);
            
    if (tw_match_port_ether_addr(&(eth->d_addr), "tw0") || is_broadcast_ether_addr(&(eth->d_addr))) {
        switch (eth_type) {
            case ETHER_TYPE_ARP:
                tw_arp_parser(buffer, "tw0");
                break;
            case ETHER_TYPE_IPv4:
                
                //udp_hdr = rte_pktmbuf_mtod(buffer->data, struct udp_hdr) //+ prev_hdr_size;?
                udp_hdr_d = buffer->data+sizeof(struct ether_hdr*)+ sizeof(struct ipv4_hdr*);
                uint8_t dst_port = (udp_hdr_d->dst_port);
                uint8_t src_port = (udp_hdr_d->src_port);
                udp_hdr_d->dst_port = (src_port);
                udp_hdr_d->src_port = (dst_port);
                udp_hdr_d->dgram_cksum = 0;
           
                ipHdr_d = buffer->data+sizeof(struct ether_hdr*);
                uint32_t dst_ip  = (ipHdr_d->dst_addr);
                uint32_t src_ip = (ipHdr_d->src_addr);
                ipHdr_d->dst_addr = (src_ip);
                ipHdr_d->src_addr = (dst_ip);
                ipHdr_d->hdr_checksum = rte_ipv4_cksum(ipHdr_d);

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
    user_params.arg_count = argc;
    user_params.arg_vals = argv;
    tw_init_global(argc,argv);
    tw_map_port_to_engine("tw0", "engine0");
    tw_launch_engine(user_app_main, (void *) &user_params, USE_ALL_ENGINES);

    return 0;
}

int user_app_main(__attribute__((unused)) void * app_params) {

    tw_rx_t * server;
    //tw_timer_t* timer_handle; //...... Move to send
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
    
    /*timer_handle = tw_timer_init(tw_loop); To be put in send
    //struct tw_sockaddr_in * addr; addr->sock_ip='172.17.250.8'; addr->sock_port=8787;
    tw_timer_bind(timer_handle)
    status = tw_timer_start(timer_handle, timer_callback_eth, 1, 1); */

    tw_run(tw_loop);
    return 0;
}
