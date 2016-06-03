#include <stdio.h>
#include <stdint.h>
#include <tw_common.h>
#include <tw_api.h>

int main(int, char **);
int user_app_main(void *);
void reply_payload(tw_rx_t *, tw_buf_t *);

uint8_t dst_port, src_port;
uint32_t dst_ip, src_ip;
struct ether_hdr * eth;
struct ipv4_hdr * ipHdr_d;
struct udp_hdr * udp_hdr_d;
uint16_t eth_type;
struct ether_addr * dst_eth;

void reply_payload(tw_rx_t * handle, tw_buf_t * buffer)
{
    eth = buffer->data;
    eth_type = tw_be_to_cpu_16(eth->ether_type);
    switch (eth_type)
    {
        case ETHER_TYPE_ARP:
            tw_arp_parser(buffer, "tw0");
            break;
        case ETHER_TYPE_IPv4:
            ipHdr_d = buffer->data + sizeof(struct ether_hdr);
            dst_ip = (ipHdr_d->dst_addr);
            src_ip = (ipHdr_d->src_addr);
            ipHdr_d->dst_addr = (src_ip);
            ipHdr_d->src_addr = (dst_ip);
            ipHdr_d->hdr_checksum = tw_ipv4_cksum(ipHdr_d);

            udp_hdr_d = (struct udp_hdr *) ipHdr_d + sizeof(struct ipv4_hdr);
            dst_port = (udp_hdr_d->dst_port);
            src_port = (udp_hdr_d->src_port);
            udp_hdr_d->dst_port = (src_port);
            udp_hdr_d->src_port = (dst_port);
            udp_hdr_d->dgram_cksum = 0;

            tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
            tw_copy_ether_addr(dst_eth, &(eth->s_addr));
            tw_send_pkt(buffer, "tw0");
            break;
    }
}

int main(int argc, char **argv)
{
    tw_init_global();
    tw_map_port_to_engine("tw0", "engine0");
    dst_eth = tw_get_ether_addr("tw0");
    user_app_main(NULL);
    return 0;
}

int user_app_main(__attribute__((unused)) void * app_params)
{
    tw_rx_t * server;
    int status;
    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP);

    server = tw_rx_init(tw_loop);
    if(server == NULL)
    {
        printf("Error in RX init\n");
        exit(1);
    }

    status = tw_rx_start(server, reply_payload);
    if(status)
    {
        printf("Error in receive start\n");
        exit(1);
    }
    tw_run(tw_loop);
    return 0;
}
