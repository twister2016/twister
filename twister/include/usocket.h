#ifndef _USOCKET_H_
#define _USOCKET_H_

#include<udp.h>

#define MAX_UDP_PAYLOAD 1460 //TODO see if this can be increased

#define	TW_SOCK_STREAM		1	// stream socket 
#define	TW_SOCK_DGRAM		2	// datagram socket 
#define	TW_SOCK_RAW			3	// raw-protocol interface 
#define	TW_SOCK_RDM			4	// reliably-delivered message 
#define	TW_SOCK_SEQPACKET	5	// sequenced packet stream

#define TW_IPPROTO_IP	0               // dummy for IP
#define TW_IPPROTO_UDP	17              // user datagram protocol
#define TW_IPPROTO_TCP	6               // tcp 
#define TW_IPPROTO_RAW	255				//For raw socket

struct socket_info{
	uint8_t type;
	uint8_t proto;
	uint32_t ip_addr;
	uint32_t port;
};

extern int maxfd;
extern struct socket_info sockets[1024];
extern int udp_socket(uint32_t,uint32_t);
extern int udp_send(int, void *,uint16_t, uint16_t, uint32_t, uint16_t);
int udp_recv(int , void *, struct sock_conn_t *);
extern void add_packet_to_udp_queue(int i,struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip,uint16_t dst_port,uint16_t src_port);
#endif
