#include <stdio.h>
#include <stdint.h>
#include <tw_common.h>
#include <stdbool.h>
#include <tw_api.h>
#include <arpa/inet.h>
#include <arplogic.h>


///variables////
uint32_t ip=0;
int arp_flag=-1;
int mac_received=0;

//////prototypes//////
int main(int, char **);
int user_app_main(void *);
void reply_payload(tw_rx_t *, tw_buf_t *);
static int console_input(int*);
void check_arp(int* no);
bool isValidIpAddress(char *ipAddress);
///////////////////////


bool isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}

static int console_input(int* dont){
    char line[1024];
    printf("\e[1;1H\e[2J");
    printf("\n ******** TWISTER ARP APPLICATION ***********\n");
    printf("\n enter IP address i.e., <xx.xx.xx.xx> or <arptable> or <exit>")
    printf("\n>>");
    while(1){
        
        fgets (line, 1024, stdin);
        line[strlen(line)-1] = 0;
        if (isValidIpAddress(line)){
            ip=tw_convert_ip_str_to_dec(line);
            arp_flag=1;
            mac_received=0; 
           
        }
        else if (strcmp(line, "exit") == 0) {
            printf ("exiting ......\n");
            exit(0);
        }
        else if (strcmp(line, "arptable") == 0) {
            tw_print_arp_table();
            printf("\n>>");
        }
        else{
            printf("command not valid\n");
            printf("\n>>");
        }

       
    }    
    return 0;
}

void reply_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    struct ether_addr * dst_eth_addr;
    struct ether_hdr * eth = buffer->data;
    struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth + 1);

        switch (tw_be_to_cpu_16(eth->ether_type)) {
            case ETHER_TYPE_ARP:
                tw_arp_parser(buffer, "tw0");
                
                struct arp_table * temp_arp_entry = tw_search_arp_table(rte_be_to_cpu_32(ip));
                if (temp_arp_entry!=NULL && mac_received==0) {
                    if (rte_be_to_cpu_16(arp_pkt->arp_op) == ARP_OP_REPLY)
                        printf("its an arp reply\n");
                    dst_eth_addr = &temp_arp_entry->eth_mac;
                    
                    printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                                dst_eth_addr->addr_bytes[0],
                                dst_eth_addr->addr_bytes[1],
                                dst_eth_addr->addr_bytes[2],
                                dst_eth_addr->addr_bytes[3],
                                dst_eth_addr->addr_bytes[4],
                                dst_eth_addr->addr_bytes[5]);
                    fprintf(stderr, "\n>>");
                    arp_flag=-1;
		            mac_received=1;
                    break;
                }
                else
                    printf("temp arp entry returned NULL\n");    
        }
    tw_free_pkt(buffer);

}
void check_arp(int* no){
    int phy_port_id = tw_eth_name_to_id("tw0");
    int arp_count=0;
    if (arp_flag==1){
        tw_construct_arp_packet(ip, phy_port_id);
     //   printf(" 1 packet sent to ip : %u count=%d\n",ip,arp_count);
        arp_flag=-1;
    }
    /*while (arp_flag!= -1){
        arp_count++;
        tw_construct_arp_packet(ip, phy_port_id);
        usleep(5000000);
        printf(" 1 packet sent to ip : %u count=%d\n",ip,arp_count);
        if ( arp_count>5 ){
            arp_flag=-1;
            break;
        }
    }*/
}

int main(int argc, char **argv) {
    tw_init_global(argc,argv);
    Printing_Enable = 0; //disable the real-time printing of Tx/Rx,
    tw_map_port_to_engine("tw0", "engine0");
    tw_launch_engine(console_input,NULL, "engine1");
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
    tw_timer_bind(timer_handle, NULL, 0, 0);
    tw_timer_start(timer_handle, check_arp, 1, 1);
    
    tw_run(tw_loop);
    return 0;
}

