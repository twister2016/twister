#ifndef _IP_H_
#define _IP_H_

#include <rte_udp.h>
#include "portconf.h"
#include "ip.h"
#define UDP_PROTO_ID	17

struct udp_conn_t
{
	uint16_t src_port;
	uint16_t dst_port;
	uint32_t src_ip;
	uint32_t dst_ip;
	
	struct udp_conn_t *next;
};


static void udp_packet_parser(struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip);
static void udp_packet_create(struct rte_mbuf *pkt, struct udp_conn_t *udp_conn);
static void udp_packet_parser(struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip)
{
	uint16_t dst_port, src_port
	struct udp_conn_t *rx_udp_conn;
	struct udp_hdr *udp_hdr_d = rte_pktmbuf_mtod(pkt, struct udp_hdr *);
	dst_port  = rte_be_to_cpu_16(udp_hdr_d->dst_port);
	src_port = rte_be_to_cpu_16(udp_hdr_d->src_port);
	
	rx_udp_conn.dst_ip = dst_ip;
	rx_udp_conn.src_ip = src_ip ;
	rx_udp_conn.dst_port = src_port;
	rx_udp_conn.src_port = dst_port;
	rx_udp_conn.next = NULL;
	
	rte_pktmbuf_adj(pkt, sizeof(struct udp_hdr));

	if(pkt->pkt.pkt_len >0)
		// Call Application function here
	else
		rte_pktmbuf_free(pkt);	
	
}

static void udp_packet_create(struct rte_mbuf *pkt, struct udp_conn_t *udp_conn);
{
	rte_pktmbuf_prepend(pkt, sizeof(struct udp_hdr));
	struct udp_hdr *udp_hdr_d = rte_pktmbuf_mtod(pkt, struct udp_hdr *);
	udp_hdr_d->src_port = rte_cpu_to_be_16(udp_conn->src_port);
	udp_hdr_d->dst_port = rte_cpu_to_be_16(udp_conn->dst_port);
	udp_hdr_d->dgram_len = rte_cpu_to_be_16(pkt->pkt.pkt_len);
	udp_hdr_d->dgram_cksum = udp_hdr_d->src_port + udp_hdr_d->dst_port + udp_hdr_d->dgram_len ;
	ip4_packet_create(pkt, UDP_PROTO_ID,udp_conn->src_ip,udp_conn->dst_ip,udp_hdr_d->dgram_len);
}