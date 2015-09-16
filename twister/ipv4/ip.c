#ifndef _IPv4_C_
#define _IPv4_C_

#include <rte_ip.h>
#include <rte_memcpy.h>
#include <rte_byteorder.h>
#include <rte_mbuf.h>
#include <rte_udp.h>
#include <portconf.h>
#include <ip.h>
#include <udp.h>
#include <eth.h>
#include <event_loop.h>

#define LOCAL_HOST_IP 2130706433     //127.0.0.1

int tw_ip4_packet_parser(struct rte_mbuf *pkt, uint8_t port_id, uint8_t prev_hdr_size, uint8_t processing_flag, void * cb_func) {
	struct ipv4_hdr *ipHdr;
	uint8_t * temp_ptr = rte_pktmbuf_mtod(pkt, uint8_t *) + (uint8_t) prev_hdr_size; //--???
	ipHdr = (struct ipv4_hdr *) temp_ptr;
	if(CHECK_IPv4_CKSUM) {
		uint16_t ipchecksum = rte_ipv4_cksum(ipHdr);
		if (ipchecksum != ipHdr->hdr_checksum) {
			rte_pktmbuf_free(pkt);
			return -1;
		}
	}
	uint32_t dst_ip  = rte_be_to_cpu_32(ipHdr->dst_addr);
	uint32_t src_ip = rte_be_to_cpu_32(ipHdr->src_addr);
	if (((dst_ip >= port_info[port_id].start_ip_addr) && (dst_ip <= (port_info[port_id].start_ip_addr + port_info[port_id].num_ip_addrs))) || dst_ip == LOCAL_HOST_IP)
	{	
		switch(ipHdr->next_proto_id)
		{
			case (UDP_PROTO_ID):
				//rte_pktmbuf_adj(pkt, sizeof(struct ipv4_hdr));
				prev_hdr_size += sizeof(struct ipv4_hdr);
				if(processing_flag == LOOP_PROCESS) {
					tw_udp_packet_parser(pkt,src_ip,dst_ip, prev_hdr_size, processing_flag, NULL);
				}
				else
				{
					void (*ip_cb_func) (struct rte_mbuf *, uint8_t) = cb_func;
					ip_cb_func(pkt, port_id);
				}
				break;
			case (TCP_PROTO_ID):
				//tcp_packet_parser(pkt);    //TODO Implement TCP
				rte_pktmbuf_free(pkt);
				break;
			default:
				rte_pktmbuf_free(pkt);
				break;
		}
	}
	else
	{
		rte_pktmbuf_free(pkt);	
	}
	return 0;
}



void tw_ip4_packet_create(struct rte_mbuf *pkt,uint8_t next_proto_id,uint32_t src_ip,uint32_t dst_ip,uint16_t length) {

	rte_pktmbuf_prepend(pkt, sizeof(struct ipv4_hdr));
	struct ipv4_hdr *ip_hdr  = rte_pktmbuf_mtod(pkt, struct ipv4_hdr *);
	ip_hdr->total_length = rte_cpu_to_be_16(length + sizeof(struct ipv4_hdr));
	ip_hdr->next_proto_id = next_proto_id;
	ip_hdr->src_addr = rte_cpu_to_be_32(src_ip);
	ip_hdr->dst_addr = rte_cpu_to_be_32(dst_ip);
	ip_hdr->version_ihl = 0x45;
	ip_hdr->time_to_live = 63;
	ip_hdr->hdr_checksum = 0;
	ip_hdr->hdr_checksum =rte_ipv4_cksum(ip_hdr);
	int port_id = tw_get_port_by_ip(src_ip);
	if(port_id < 0) {
		rte_pktmbuf_free(pkt);
		return;
	}
	tw_eth_pkt_ctor(pkt, port_id, ETHER_TYPE_IPv4, dst_ip );
}

#endif
