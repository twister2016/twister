#include <stdio.h>
#include <stdint.h>
#include <tw_common.h>
#include <stdbool.h>
#include <tw_api.h>
#include <arpa/inet.h>

///variables////
char* ip;
int arp_flag = -1;

//////prototypes//////
int main(int, char **);
int user_app_main(void *);
void reply_payload(tw_rx_t *, tw_buf_t *);
void check_arp();
bool isSameNetwork(char * );
bool isValidIpAddress(char *ipAddress);
///////////////////////
bool isSameNetwork(char* ip){
    uint32_t ipp = tw_convert_ip_str_to_dec(ip);
    uint32_t desAnd = ipp & tw_get_subnet_mask("tw0");
    uint32_t souAnd = tw_get_ip_addr("tw0") & tw_get_subnet_mask("tw0");
    return desAnd == souAnd; 
}

bool isValidIpAddress(char *ipAddress) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}
void reply_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_addr * dst_eth_addr;
    struct ether_hdr * eth = buffer->data;
    struct arp_hdr * arp_pkt =buffer->data + sizeof(struct ether_hdr);

    switch (tw_be_to_cpu_16(eth->ether_type)) {
        case ETHER_TYPE_ARP:
            if (arp_flag == 1) {
                tw_arp_parser(buffer, "tw0");
                struct ether_addr * mac_addr = tw_search_arp_entry(ip);
                if (mac_addr != NULL) {
                    dst_eth_addr =  mac_addr;
                    printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                            dst_eth_addr->addr_bytes[0],
                            dst_eth_addr->addr_bytes[1],
                            dst_eth_addr->addr_bytes[2],
                            dst_eth_addr->addr_bytes[3],
                            dst_eth_addr->addr_bytes[4],
                            dst_eth_addr->addr_bytes[5]);
                    arp_flag = -1;
                    exit(0);
                }
            }
    }
    tw_free_pkt(buffer);

}

void check_arp() {
    static arp_resolv_count = 0;    
    arp_resolv_count++;
    tw_send_arp_request(tw_convert_ip_str_to_dec(ip), "tw0");
    if ( arp_flag == 1 && arp_resolv_count > 4 ) {
        printf("arp not resolved.\n");
        exit(1);
    }    
}

int main(int argc, char **argv) {
    tw_init_global();
    Printing_Enable = 0; //disable the real-time printing of Tx/Rx,
    tw_map_port_to_engine("tw0", "engine0");
    if (argc == 2) {
        ip = argv[1];    
        if (isValidIpAddress(ip)){
            if (isSameNetwork(ip))                                                                                                    
                arp_flag = 1;
            else{
                printf("Cannot resolve address outside of network.\n");
                exit(1);
            }   
        }
        else{
            printf("Host address not valid.\n");
            exit(1);
        }
    }
    else{
        printf("Please specify twarp <x.x.x.x> \n");
        exit(1);
    }
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
