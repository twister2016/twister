#include <stdio.h>
#include <stdint.h>
#include <tw_common.h>
#include <stdbool.h>
#include <tw_api.h>
#include <arpa/inet.h>
#include <arplogic.h>


struct ether_addr requestmac = {
    .addr_bytes[1] = 0x09,
    .addr_bytes[2] = 0xc0,
    .addr_bytes[3] = 0x84,
    .addr_bytes[4] = 0xd2,
    .addr_bytes[5] = 0x97,
    .addr_bytes[0] = 0x02,//02:09:c0:84:d2:97
};

///variables////
#define BUFF_SIZE 42 //sizeof eth hdr = 14 + sizeof arp hdr = 28

int rarp_flag = -1;
int rarp_resolved_reply = 0;

//////prototypes//////
int main(int, char **);
int user_app_main(void *);
void reply_payload(tw_rx_t *, tw_buf_t *);
static int console_input(int*);
void check_arp(int* no);
bool isValidIpAddress(char *ipAddress);
///////////////////////

bool isValidIpAddress(char *ipAddress) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

static int console_input(int* dont) {
    char line[1024];
    printf("\e[1;1H\e[2J");
    printf("\n ******** TWISTER RARP APPLICATION ***********\n");
    printf("\n enter <send rarp> or <exit>");
    printf("\n>>");
    while (1) {

        fgets(line, 1024, stdin);
        line[strlen(line) - 1] = 0;
        if (strcmp(line, "send rarp") == 0) {
            rarp_flag = 1;
            rarp_resolved_reply = 0;
            uint8_t ip_heartbeat = 0;
            while (rarp_resolved_reply != 1) { 
                /*if we don't received reply in 4 seconds,
                  let the user know that reverse arp is not resolved.*/ 
                ip_heartbeat++;
                usleep(1000000);
                if (ip_heartbeat > 4) {
                    printf("RARP not resolved\n");
                    break;
                }
            }
            printf("\n>>");

        } else if (strcmp(line, "exit") == 0) {
            printf("exiting ......\n");
            exit(0);
        } else {
            printf("command not valid\n");
            printf("\n>>");
        }



    }
    return 0;
}

void reply_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_addr * dst_eth_addr;
    struct ether_hdr * eth = buffer->data;
    uint32_t target_ip = 0;
    uint32_t sender_ip = 0;
    
    struct arp_hdr * arp_pkt = buffer->data + sizeof (struct ether_hdr);
    switch (tw_be_to_cpu_16(eth->ether_type)) {
        case ETHER_TYPE_RARP:
            target_ip = arp_pkt->arp_data.arp_tip;
            sender_ip = arp_pkt->arp_data.arp_sip;
            struct in_addr ip_addr;
            ip_addr.s_addr = (target_ip);
            if (rarp_resolved_reply == 0) { /* if user is waiting for reply and we have
                                             * received the reply, print it
                                             */
                printf("Resolved IP for MAC(%02X:%02X:%02X:%02X:%02X:%02X) = %s \n ",
                    eth->d_addr.addr_bytes[0],
                    eth->d_addr.addr_bytes[1],
                    eth->d_addr.addr_bytes[2],
                    eth->d_addr.addr_bytes[3],
                    eth->d_addr.addr_bytes[4],
                    eth->d_addr.addr_bytes[5],  inet_ntoa(ip_addr));
            
                rarp_resolved_reply = 1;                        
            }
            rarp_flag = -1;
            break;
    }

    tw_free_pkt(buffer);

}

void check_arp(int* no) {
    if (rarp_flag == 1) {

        int port_id = tw_eth_name_to_id("tw0");
        tw_buf_t * tx_buf = tw_new_buffer(BUFF_SIZE);
        struct ether_hdr* eth = tx_buf->data;
        struct arp_hdr *rarp = tx_buf->data + sizeof (struct ether_hdr);
        rarp->arp_op = rte_cpu_to_be_16(ARP_OP_REVREQUEST);
        rarp->arp_pro = rte_cpu_to_be_16(ETHER_TYPE_IPv4);
        rarp->arp_pln = 4;
        rarp->arp_hln = 6;
        rarp->arp_hrd = rte_cpu_to_be_16(ARP_HRD_ETHER);
        ether_addr_copy(tw_get_ether_addr("tw0"), &(rarp->arp_data.arp_sha));
        ether_addr_copy(tw_get_ether_addr("tw0"), &(rarp->arp_data.arp_tha));
        rarp->arp_data.arp_sip = 0;
        rarp->arp_data.arp_tip = 0;

        eth->ether_type = tw_cpu_to_be_16(ETHER_TYPE_RARP);
        tw_copy_ether_addr(&(broadcastmac), &(eth->d_addr));
        tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
        tw_send_pkt(tx_buf, "tw0");
        tw_free(tx_buf);

        rarp_flag = -1;
    }
}

int main(int argc, char **argv) {
    tw_init_global(argc, argv);
    Printing_Enable = 0; //disable the real-time printing of Tx/Rx,
    tw_map_port_to_engine("tw0", "engine0");
    tw_launch_engine(console_input, NULL, "engine1");
    user_app_main(NULL);
}

int user_app_main(__attribute__((unused)) void * app_params) {

    tw_rx_t * server;
    tw_timer_t * timer_handle;
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
    timer_handle = tw_timer_init(tw_loop);
    tw_timer_start(timer_handle, check_arp, 1000);

    tw_run(tw_loop);
    return 0;
}
