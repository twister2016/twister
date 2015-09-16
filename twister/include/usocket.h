#ifndef _USOCKET_H_
#define _USOCKET_H_

#include <rte_mbuf.h>
#include <mempoolconf.h>

#define MAX_UDP_PAYLOAD 1460 //TODO see if this can be increased
#define MAX_SOCK_FD		1024

#define	TW_SOCK_STREAM		1	// stream socket 
#define	TW_SOCK_DGRAM		2	// datagram socket 
#define	TW_SOCK_RAW			3	// raw-protocol interface 
#define	TW_SOCK_RDM			4	// reliably-delivered message 
#define	TW_SOCK_SEQPACKET	5	// sequenced packet stream

#define TW_IPPROTO_IP	0               // dummy for IP
#define TW_IPPROTO_UDP	17              // user datagram protocol
#define TW_IPPROTO_TCP	6               // TCP 
#define TW_IPPROTO_RAW	255				//For raw socket

struct socket_info{
	uint8_t type;
	uint8_t proto;
	uint32_t ip_addr;
	uint32_t port;
};

struct sock_conn_t
{
	uint16_t src_port;
	uint16_t dst_port;
	uint32_t src_ip;
	uint32_t dst_ip;
	uint16_t l4_proto;
};

struct tw_sockaddr_in {   //TODO standardise
	uint32_t sock_ip;
	uint16_t sock_port;
};

extern int maxfd;
extern struct socket_info udp_sockets[MAX_SOCK_FD];

extern int tw_udp_socket(uint32_t,uint32_t);
extern int tw_udp_send(int, tw_buf_t *, uint16_t, uint16_t, struct tw_sockaddr_in *);
extern int tw_udp_recv(int, struct tw_sockaddr_in *, tw_buf_t *);
extern void tw_add_packet_to_udp_queue(int, struct rte_mbuf *, uint32_t, uint16_t);
#endif
