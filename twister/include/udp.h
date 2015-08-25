#ifndef _UDP_H_
#define _UDP_H_

#include <portconf.h>
#include <ip.h>
#include <event_loop.h>
#include <usocket.h>

#define UDP_PROTO_ID	17
#define TCP_PROTO_ID	06

extern uint8_t udp_hdr_size;
extern uint8_t tcp_hdr_size;

extern void udp_packet_parser(struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip, uint8_t, uint8_t, void *);
extern void udp_packet_create(struct rte_mbuf *pkt, struct sock_conn_t *udp_conn); 

#endif
