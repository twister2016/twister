#ifndef _USOCKET_H_
#define _USOCKET_H_
/**
 * @file
 *
 * Sockets Helpers in Twister
 */
#include<udp.h>

#define MAX_UDP_PAYLOAD 1460 //TODO see if this can be increased
struct socket_info{
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
