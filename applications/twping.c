#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdint.h>
#include <tw_common.h>
#include <tw_api.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include<signal.h>
#include<unistd.h>


#define ICMP_PROTO_ID	1
#define BUFF_SIZE 74


static int console_input(void*);
int user_app_main(void *);
void pkt_rx(tw_rx_t *, tw_buf_t *);
void pkt_tx(tw_tx_t *);
bool isValidIpAddress(char *ipAddress);
int main(int, char **);
char** argvector; int argcount;
struct ether_hdr * eth;
struct ipv4_hdr * ip;
struct icmp_echo* icmp;
int ping=-1,arp_counter=0;
uint32_t ping_ip,ping_count, reply_count=0;
double time_ms, t1 ,t2, total_init_time;
uint16_t eth_type;
static struct ether_addr * dst_eth_addr;
uint32_t dst_ip, src_ip;

double current_timestamp() {
    clock_t start = clock();
    double elapsed = (double)(start) * 1000.0 / CLOCKS_PER_SEC;
     return elapsed;
}

void sig_handler(int signo)
{
  if (signo == SIGINT){
     ping=-1;
     double total_end_time=current_timestamp();
     struct in_addr ip_addr;
     ip_addr.s_addr = tw_cpu_to_be_32(ping_ip);
     printf(" --- %s ping statistics ---\n", inet_ntoa(ip_addr));
     double total_ping_time =  total_end_time-total_init_time;
     float loss;
     int packet_transmitted=ping_count;
     if (ping_count > 0)
         loss = (1-(reply_count/(float)ping_count))*100;  
     else if (ping_count == 0){
        if (arp_counter > 0){
            loss = (double)(1-((reply_count/(float)arp_counter)))*100;  
            packet_transmitted = arp_counter;
        }
        else
            loss =0;
     } 
     printf("%d packets transmitted, %d received, %.2f%% packet loss, time %.0lfms\n",packet_transmitted, reply_count, loss,total_ping_time);
     exit(1);
  }
}

bool isValidIpAddress(char *ipAddress)
{
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
    return result != 0;
}
bool isSameNetwork(uint32_t ip){
    uint32_t desAnd = ip & tw_get_subnet_mask("tw0");
    uint32_t souAnd = tw_get_ip_addr("tw0") & tw_get_subnet_mask("tw0");
    return desAnd == souAnd; 

}

void pkt_rx(tw_rx_t * handle, tw_buf_t * buffer) {
	    eth = buffer->data;
            ip = buffer->data+sizeof(struct ether_hdr);
            switch (ip->next_proto_id) {
                case ICMP_PROTO_ID:
                    icmp = buffer->data+sizeof(struct ether_hdr)+sizeof(struct ipv4_hdr);
		    if(icmp->type == 0)
		    {
                    	double t1 = current_timestamp();
                    	time_ms = t1 - t2; //time difference between packet sent and received
                    	struct in_addr ip_addr;
                    	ip_addr.s_addr = ip->src_addr;
                    	printf("64 bytes from %s: icmp_seq=%d ttl=%d time=%fms\n", inet_ntoa(ip_addr),icmp->sequence, ip->time_to_live,time_ms);
                    	reply_count++;
                    	break;
            	     }
		    if(icmp->type == 8)
		    {

                    	icmp->type=0;//for icmp reply type is zero
                    	icmp->checksum=0;
                    	icmp->checksum=tw_calcsum((unsigned short*)icmp, sizeof(struct icmp_echo)); 
                	dst_ip  = (ip->dst_addr);
                	src_ip = (ip->src_addr);
               		ip->dst_addr = (src_ip);
                	ip->src_addr = (dst_ip);
                	ip->hdr_checksum = tw_ipv4_cksum(ip);
                	tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
                	tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
                	tw_send_pkt(buffer, "tw0");
                	break;
		     }
		}
    tw_free_pkt(buffer);
    return;
}

void pkt_tx(tw_tx_t * handle) {
    if(ping==1){
        if (dst_eth_addr == NULL )
        {  
            uint32_t first_hop_ip = ping_ip;    
            if (isSameNetwork(ping_ip)!=1)
                    first_hop_ip = tw_get_gateway_ip("tw0");
            struct arp_table * temp_arp_entry = tw_search_arp_table(tw_be_to_cpu_32(first_hop_ip));
            if(temp_arp_entry == NULL) {
                tw_send_arp_request(first_hop_ip, "tw0");
                arp_counter++;
                if (arp_counter > 3) {
                    struct in_addr ip_addr;
                    ip_addr.s_addr =tw_cpu_to_be_32(tw_get_ip_addr("tw0"));
                    printf("From %s icmp_seq=%d Destination Host Unreachable\n",inet_ntoa(ip_addr), arp_counter-3);
                    return;
                }    
            }
            else
                dst_eth_addr = &temp_arp_entry->eth_mac;
        }
        else{
            tw_buf_t * tx_buf = tw_new_buffer(BUFF_SIZE);
            eth = tx_buf->data;
            ip  = tx_buf->data+sizeof(struct ether_hdr);
            icmp = tx_buf->data+sizeof(struct ether_hdr)+sizeof(struct ipv4_hdr);
            icmp->type		= 8;		/* icmp echo */
            icmp->code		= 0;		/* only valid value for echo or echo reply */
            icmp->identifier	= 0x1337;	/* the id we'll be using to distinguish our data from other icmp packets */
            ping_count++;									// ^^^ Actually it won't. It is now distinguished with the ip header
            icmp->sequence		= ping_count;		/* our initial sequence will be zero
                /* just like IP we have to zero the checksum before we calculate it */
            t2=current_timestamp();
            icmp->checksum = 0;
                /* now we can calculate the checksum */
            icmp->checksum = tw_calcsum((unsigned short*)icmp, sizeof(struct icmp_echo));
            ip->total_length = tw_cpu_to_be_16(tx_buf->size - sizeof(struct ether_hdr));
            ip->next_proto_id = ICMP_PROTO_ID;
            ip->src_addr = tw_cpu_to_be_32(tw_get_ip_addr("tw0"));

            ip->dst_addr = tw_cpu_to_be_32(ping_ip);
            ip->version_ihl = 0x45;
            ip->time_to_live = 63;
            ip->hdr_checksum = 0;
            ip->hdr_checksum =tw_ipv4_cksum(ip);
            eth->ether_type = tw_cpu_to_be_16(ETHER_TYPE_IPv4);
            tw_copy_ether_addr(dst_eth_addr, &(eth->d_addr));
            tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
            tw_send_pkt(tx_buf, "tw0");
            tw_free(tx_buf);
        }
    }
}
int main(int argc, char **argv) {
    argvector = argv ;
    argcount = argc;
    tw_init_global();
    tw_map_port_to_engine("tw0", "engine0");
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    if (argcount>2){
        printf("Please specify twping <x.x.x.x> \n");
        exit(1);}
    if (argcount<2) {
        printf("Please specify twping <x.x.x.x> \n");
        exit(1);}
    else if (argcount ==2 ){
        char* line=argv[1]; 
        if (isValidIpAddress(line)){
            ping_ip=tw_convert_ip_str_to_dec(line);
            ping=1;
            total_init_time=current_timestamp();
            struct in_addr ip_addr;
            ip_addr.s_addr =tw_cpu_to_be_32(tw_get_ip_addr("tw0"));
            printf("PING %s (%s) 56(84) bytes of data.\n", line, inet_ntoa(ip_addr)); 
        }   
        else{
            printf("command not valid\n");
            exit(1);
        }
    }   
    user_app_main(NULL);
    return 0;
}
int user_app_main(__attribute__((unused)) void * app_params) {

    tw_rx_t * rx_handle;
    tw_timer_t * timer_handle;
    int status;
    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP);

    rx_handle = tw_rx_init(tw_loop);
    if (rx_handle == NULL) {
        printf("Error in RX init\n");
        exit(1);
    }
    status = tw_rx_start(rx_handle, pkt_rx);
    if (status) {
        printf("Error in receive start\n");
        exit(1);
    }

    timer_handle = tw_timer_init(tw_loop);
    tw_timer_start(timer_handle,pkt_tx , 1000);
    tw_run(tw_loop);
    return 0;
}
