#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <tw_common.h>
#include <tw_api.h>
#include <stats.h>

/* Maximum size UDP send is (64K - 1) - IP and UDP header sizes */
//#define MAX_UDP_BLOCKSIZE (65535 - 8 - 20)
#define MAX_UDP_BLOCKSIZE 1500

enum {
    IENONE = 0,             // No error
                /* Parameter errors */
    IEUDPBLOCKSIZE=1, //UDP block size exceeds
    PORTNUMBERWRONG = 2,           // Iperf must either be a client (-c) or server (-s)
    IENOROLE= 3,           // Iperf must either be a client (-c) or server (-s)
};
struct iperf_test {                                                                                                                          
    int role; //1 for server, 2 for client
    uint32_t protocol_id;// 1 for ethernet, 2 for udp
    uint32_t server_port; 
    uint32_t client_port; 
    uint32_t server_ip; 
    uint32_t client_ip; 
    uint32_t bytes; 
    struct ether_addr * server_mac;
    struct ether_addr * client_mac;
    struct ether_hdr * eth;
    struct ipv4_hdr * ip;
    struct udp_hdr * udp;
    uint8_t test_runtime;
    tw_buf_t * tx_buf;
};
/* display usage */                                                                                                                 
const char usage_longstr[] = "Usage: iperf [-s|-c host] [options]\n"
                           "       iperf [-h|--help] [-v|--version]\n\n"
                           "Server or Client:\n"
                           "  -p, --port      #         server port to listen on/connect to\n"
                           "  -B, --bind      <host>    bind to a specific interface\n"
                           "  -h, --help                show this message and quit\n"
                           "Server specific:\n"
                           "  -s, --server              run in server mode\n"
                           "Client specific:\n"
                           "  -c, --client    <host>    run in client mode, connecting to <host>\n";


char *
iperf_strerror(int i_errno)
{
    static char errstr[256];
    int len, perr, herr;
    perr = herr = 0;
    len = sizeof(errstr);
    memset(errstr, 0, len);
    switch (i_errno) {
        case IENONE:
            snprintf(errstr, len, "no error");
            break;
        case IEUDPBLOCKSIZE:
            snprintf(errstr, len, "block size too large (maximum = %d bytes)", MAX_UDP_BLOCKSIZE);
            break;
        case IENOROLE:
            snprintf(errstr, len, "must either be a client (-c) or server (-s)");
            break;
    }
    return errstr;
}    

//void iperf_err(struct iperf_test *test, const char *format, ...) __attribute__ ((format(printf,2,3)));


void
iperf_err(struct iperf_test *test, const char *format, ...)
{
    va_list argp;
    char str[1000];
    va_start(argp, format);
    vsnprintf(str, sizeof(str), format, argp);

//    if (test != NULL && test->json_output && test->json_top != NULL)
//        cJSON_AddStringToObject(test->json_top, "error", str);
//    else
//        if (test && test->outfile) {
//            fprintf(test->outfile, "iperf3: %s\n", str);
//        }
//        else {
            fprintf(stderr, "iperf3: %s\n", str);
//        }
        va_end(argp);
}


void twiperf_usage()
{
fprintf(stderr, usage_longstr);

}        

/* 
    96 bit (12 bytes) pseudo header needed for udp header checksum calculation 
*/
/*struct pseudo_header
{
    uint32_t source_address;
    uint32_t dest_address;
    uint8_t placeholder;
    uint8_t protocol;
    uint16_t udp_length;
};
 
unsigned short csum(unsigned short *ptr,int nbytes) 
{
    register long sum;
    unsigned short oddbyte;
    register short answer;
 
    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }
 
    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
     
    return(answer);
}
 
int udp_checksum (struct ipv4_hdr* ip, struct udp_hdr* udp )
{
    //Create a raw socket of type IPPROTO
    int s = socket (AF_INET, SOCK_RAW, IPPROTO_RAW);
     
    if(s == -1)
    {
        //socket creation failed, may be because of non-root privileges
        perror("Failed to create raw socket");
        exit(1);
    }
     
    //Datagram to represent the packet
    char datagram[4096] , source_ip[32] , *data , *pseudogram;
     
    //zero out the packet buffer
    memset (datagram, 0, 4096);
     
    //IP header
    struct iphdr *iph = (struct iphdr *) datagram;
     
    //UDP header
    struct udphdr *udph = (struct udphdr *) (datagram + sizeof (struct ip));
     
    struct sockaddr_in sin;
    struct pseudo_header psh;
     
    //Data part
    data = datagram + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data , "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
     
    //some address resolution
    strcpy(source_ip , "192.168.1.2");
     
    sin.sin_family = AF_INET;
    sin.sin_port = htons(80);
    sin.sin_addr.s_addr = inet_addr ("192.168.1.1");
     
    //Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof (struct iphdr) + sizeof (struct udphdr) + strlen(data);
    iph->id = htonl (54321); //Id of this packet
    iph->frag_off = 0;
    iph->ttl = 255;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;      //Set to 0 before calculating checksum
    iph->saddr = inet_addr ( source_ip );    //Spoof the source ip address
    iph->daddr = sin.sin_addr.s_addr;
     
    //Ip checksum
    iph->check = csum ((unsigned short *) datagram, iph->tot_len);
     
    //UDP header
    udph->source = htons (6666);
    udph->dest = htons (8622);
    udph->len = htons(8 + strlen(data)); //tcp header size
    udph->check = 0; //leave checksum 0 now, filled later by pseudo header
     
    //Now the UDP checksum using the pseudo header
    psh.source_address = ip->src_addr;
    psh.dest_address = dst_addr;
    psh.placeholder = 0;
    psh.protocol = 17;
    psh.udp_length = 12;//htons(sizeof(struct udphdr) + strlen(data) );
     
    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
    pseudogram = malloc(psize);
     
    memcpy(pseudogram , (char*) &psh , sizeof (struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header) , udph , sizeof(struct udphdr) + strlen(data));
     
    udph->check = csum( (unsigned short*) pseudogram , psize);
     
    //loop if you want to flood :)
    //while (1)
    {
        //Send the packet
        if (sendto (s, datagram, iph->tot_len ,  0, (struct sockaddr *) &sin, sizeof (sin)) < 0)
        {
            perror("sendto failed");
        }
        //Data send successfully
        else
        {
            printf ("Packet Send. Length : %d \n" , iph->tot_len);
        }
    }
     
    return 0;
}
 
//Complete */

static inline uint16_t
get_16b_sum(uint16_t *ptr16, uint32_t nr)
{
	uint32_t sum = 0;
	while (nr > 1)
	{
		sum +=*ptr16;
		nr -= sizeof(uint16_t);
		ptr16++;
		if (sum > UINT16_MAX)
			sum -= UINT16_MAX;
	}

	/* If length is in odd bytes */
	if (nr)
		sum += *((uint8_t*)ptr16);

	sum = ((sum & 0xffff0000) >> 16) + (sum & 0xffff);
	sum &= 0x0ffff;
	return (uint16_t)sum;
}

static inline uint16_t
get_ipv4_psd_sum (struct ipv4_hdr * ip_hdr)
{
	/* Pseudo Header for IPv4/UDP/TCP checksum */
	union ipv4_psd_header {
		struct {
			uint32_t src_addr; /* IP address of source host. */
			uint32_t dst_addr; /* IP address of destination host(s). */
			uint8_t  zero;     /* zero. */
			uint8_t  proto;    /* L4 protocol type. */
			uint16_t len;      /* L4 length. */
		} __attribute__((__packed__));
		uint16_t u16_arr[0];
	} psd_hdr;

	psd_hdr.src_addr = ip_hdr->src_addr;
	psd_hdr.dst_addr = ip_hdr->dst_addr;
	psd_hdr.zero     = 0;
	psd_hdr.proto    = ip_hdr->next_proto_id;
	psd_hdr.len      = rte_cpu_to_be_16((uint16_t)(rte_be_to_cpu_16(ip_hdr->total_length)
				- sizeof(struct ipv4_hdr)));
	return get_16b_sum(psd_hdr.u16_arr, sizeof(psd_hdr));
}









