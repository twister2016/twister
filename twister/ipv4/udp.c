#include <rte_udp.h>
#include <rte_memcpy.h>
#include <rte_byteorder.h>
#include <rte_mbuf.h>

#include <portconf.h>
#include<usocket.h>
#include <udp.h>
#include <ip.h>

void udp_packet_parser(struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip)
{
	uint16_t dst_port, src_port;
	struct udp_hdr *udp_hdr_d = rte_pktmbuf_mtod(pkt, struct udp_hdr *);
	dst_port  = rte_be_to_cpu_16(udp_hdr_d->dst_port);
	src_port = rte_be_to_cpu_16(udp_hdr_d->src_port);	
	int i=0;
    struct socket_info * sockptr = NULL;
    for (i=0;i<=maxfd;i++){
        sockptr =&sockets[i];
        if(sockptr->port==dst_port){
            break;
            }
    }	
	if (i==maxfd){
		rte_pktmbuf_free(pkt);
		return;
	}

	rte_pktmbuf_adj(pkt, sizeof(struct udp_hdr));
	add_packet_to_queue(i,pkt,src_ip,dst_ip,src_port,dst_port);
}

void udp_packet_create(struct rte_mbuf *pkt, struct udp_conn_t *udp_conn)
{
	struct udp_hdr *udp_hdr_d;
	rte_pktmbuf_prepend(pkt, sizeof(struct udp_hdr));
	udp_hdr_d = rte_pktmbuf_mtod(pkt, struct udp_hdr *);
	udp_hdr_d->src_port = udp_conn->src_port;
	udp_hdr_d->dst_port = udp_conn->dst_port;
	udp_hdr_d->dgram_len = rte_cpu_to_be_16(pkt->pkt_len);
	udp_hdr_d->dgram_cksum = udp_hdr_d->src_port + udp_hdr_d->dst_port + udp_hdr_d->dgram_len ;
	ip4_packet_create(pkt, UDP_PROTO_ID,udp_conn->src_ip,udp_conn->dst_ip,udp_hdr_d->dgram_len);
    
}

