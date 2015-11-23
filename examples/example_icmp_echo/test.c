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


#define ICMP_PROTO_ID	1
#define DES_ADDR 181169931
#define BUFF_SIZE 74

struct icmp_echo{
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	unsigned short identifier;
	unsigned short sequence;
	long long time; /* we're going to send data MTU bytes at a time */
};

unsigned short calcsum(unsigned short *buffer, int length);
int main(int, char **);
int user_app_main(void *);
void pkt_rx(tw_rx_t *, tw_buf_t *);
void pkt_tx(tw_tx_t *);
struct ether_hdr * eth;
struct ipv4_hdr * ip;
struct icmp_echo* icmp;
int phy_port_id;
int finishing = 0;
double time_ms, t1 ,t2;
uint16_t eth_type;
static struct ether_addr * dst_eth_addr;

double  current_timestamp() {
    clock_t start = clock();
    double elapsed = (double)(start) * 1000.0 / CLOCKS_PER_SEC;
     return elapsed;
}
void sleepWrapper (unsigned int secs) {
    unsigned int left = secs;
    while ((left > 0) && (!finishing)) // Don't continue if signal has
        left = sleep (left);           //   indicated exit needed.
}
unsigned short calcsum(unsigned short *buffer, int length){
	unsigned long sum;

	// initialize sum to zero and loop until length (in words) is 0
	for (sum=0; length>1; length-=2) // sizeof() returns number of bytes, we're interested in number of words
		sum += *buffer++;	// add 1 word of buffer to sum and proceed to the next

	// we may have an extra byte
	if (length==1)
		sum += (char)*buffer;

	sum = (sum >> 16) + (sum & 0xFFFF);  // add high 16 to low 16
	sum += (sum >> 16);		     // add carry
	return ~sum;
}
void pkt_rx(tw_rx_t * handle, tw_buf_t * buffer) {
   eth = buffer->data;
    eth_type = tw_be_to_cpu_16(eth->ether_type);
           switch (eth_type) {
            case ETHER_TYPE_ARP:
                tw_arp_parser(buffer, "tw0");
                break;
            case ETHER_TYPE_IPv4:
                icmp = buffer->data+sizeof(struct ether_hdr*);
                time_ms= icmp->time;
                //printf("time 1 milliseconds: %lld\n", time_ms);
                double t1=current_timestamp();
                time_ms=t1-t2;
                //printf("milliseconds current: %lld\n", t);
                printf("milliseconds difference: %f\n", time_ms);
                sleepWrapper(1);
                break;
           }
    tw_free_pkt(buffer);
    return;
}
void pkt_tx(tw_tx_t * handle) {

    if (unlikely(dst_eth_addr) == NULL) {
        struct arp_table * temp_arp_entry = tw_search_arp_table(tw_be_to_cpu_32(DES_ADDR));
        if(temp_arp_entry == NULL) {
            tw_construct_arp_packet(DES_ADDR, phy_port_id);
        }
        else
            dst_eth_addr = &temp_arp_entry->eth_mac;
    }
    else {
    tw_buf_t * tx_buf = tw_new_buffer(BUFF_SIZE);
    eth = tx_buf->data;
    ip  = (struct ipv4_hdr* )(eth + 1);
    icmp = (struct icmp_echo* )(ip + 1);
    icmp->type		= 8;		/* icmp echo */
    icmp->code		= 0;		/* only valid value for echo or echo reply */
    icmp->identifier	= 0x1337;	/* the id we'll be using to distinguish our data from other icmp packets */
									// ^^^ Actually it won't. It is now distinguished with the ip header
    icmp->sequence		= 0;		/* our initial sequence will be zero
	/* just like IP we have to zero the checksum before we calculate it */
    icmp->time=0;
    icmp->time=current_timestamp();
    t2=icmp->time;
    icmp->checksum = 0;

	/* now we can calculate the checksum */
    icmp->checksum = calcsum((unsigned short*)icmp, sizeof(struct icmp_echo));
    ip->total_length = tw_cpu_to_be_16(tx_buf->size - sizeof(struct ether_hdr));
    ip->next_proto_id = ICMP_PROTO_ID;
    ip->src_addr = tw_cpu_to_be_32(tw_get_ip_addr("tw0"));

    ip->dst_addr = tw_cpu_to_be_32(DES_ADDR);
    ip->version_ihl = 0x45;
    ip->time_to_live = 63;
    ip->hdr_checksum = 0;
    ip->hdr_checksum =tw_ipv4_cksum(ip);
    eth->ether_type = tw_cpu_to_be_16(ETHER_TYPE_IPv4);
    tw_copy_ether_addr(dst_eth_addr, &(eth->d_addr));
    tw_copy_ether_addr(tw_get_ether_addr("tw0"), &(eth->s_addr));
    tw_send_pkt(tx_buf, "tw0");
    sleepWrapper(1);
    tw_free(tx_buf);
    }
}
int main(int argc, char **argv) {
    tw_init_global(argc, argv);
    tw_map_port_to_engine("tw0", "engine0");
    phy_port_id = tw_eth_name_to_id("tw0");
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
    tw_timer_bind(timer_handle, NULL, 0, 0);
    tw_timer_start(timer_handle,pkt_tx , 1, 1);
    tw_run(tw_loop);
    return 0;
}