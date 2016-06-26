#include <stdio.h>
#include <stdint.h>
#include <tw_common.h>
#include <tw_api.h>

#define ICMP_PROTO_ID	1
int main(int, char **);
int user_app_main(void *);
void reply_payload(tw_rx_t *, tw_buf_t *);

uint8_t dst_port, src_port;
uint32_t dst_ip, src_ip;
struct ether_hdr * eth;
struct ipv4_hdr * ipHdr_d;
struct udp_hdr * udp_hdr_d;
uint16_t eth_type;
struct ether_addr * src_tw0_eth;

struct icmp_echo* ICMP;
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
            if(ipHdr_d->next_proto_id == ICMP_PROTO_ID)
            {
                ICMP = buffer->data + sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr);
                ICMP->type = 0; //for icmp reply type is zero
                ICMP->checksum = 0;
                ICMP->checksum = tw_calcsum((unsigned short*) ICMP, sizeof(struct icmp_echo));
            }
            dst_ip = (ipHdr_d->dst_addr);
            src_ip = (ipHdr_d->src_addr);
            ipHdr_d->dst_addr = (src_ip);
            ipHdr_d->src_addr = (dst_ip);
            ipHdr_d->hdr_checksum = tw_ipv4_cksum(ipHdr_d);

            tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
            tw_copy_ether_addr(src_tw0_eth, &(eth->s_addr));
            tw_send_pkt(buffer, "tw0");
            break;
    }
}

int main(int argc, char **argv)
{
    tw_init_global();
    Printing_Enable = 0;
    tw_map_port_to_engine("tw0", "engine0");
    src_tw0_eth = tw_get_ether_addr("tw0");
    printf("\n Ping Echo is Running!!\n");
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
