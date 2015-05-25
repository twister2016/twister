#ifndef _IP_H_
#define _IP_H_

#include <rte_udp.h>
#include "portconf.h"


static void udp_packet_parser(struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip);
static void udp_packet_parser(struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip)
{
	uint16_t dst_port, src_port
	struct udp_conn_t *rx_udp_conn;
	struct udp_hdr *udp_hdr_d = rte_pktmbuf_mtod(pkt, struct udp_hdr *);
	dst_port  = rte_be_to_cpu_16(udp_hdr_d->dst_port);
	src_port = rte_be_to_cpu_16(udp_hdr_d->src_port);
	
	rx_udp_conn.remote_ip = dst_ip;
	rx_udp_conn.local_ip = src_ip ;
	rx_udp_conn.remote_port = src_port;
	rx_udp_conn.local_port = dst_port;
	rx_udp_conn.next = NULL;
	
	rte_pktmbuf_adj(pkt, sizeof(struct udp_hdr));

	if(pkt->pkt.pkt_len >0)
		AppRx(pkt, &rx_udp_conn);
	else
		rte_pktmbuf_free(pkt);	
	
}

