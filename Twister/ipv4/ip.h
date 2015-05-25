#ifndef _IP_H_
#define _IP_H_

#include <rte_ip.h>
#include "portconf.h"
#include "udp.h"

static void ip4_packet_parser(struct rte_mbuf *pkt,uint8_t port_id);
static void ip4_packet_parser(struct rte_mbuf *pkt,uint8_t port_id)
{
	struct ip4_hdr *ipHdr;
	ipHdr = rte_pktmbuf_mtod(pkt, struct ip4_hdr *);
    uint16_t ipchecksum = rte_ipv4_cksum(ipHdr);	
    if (ipchecksum == ipHdr->hdr_checksum && ipHdr->version_ihl > 20 && ipHdr->time_to_live >= 0)
	{
       switch(ipHdr->next_proto_id)
		{
			uint32_t dst_ip  = rte_be_to_cpu_32(ipHdr->dst_addr);
			uint32_t src_ip = rte_be_to_cpu_32(ipHdr->src_addr);
			if (local_ip == port_info[port_id].ip_addr || local_ip == 2130706433) 
			{	
				rte_pktmbuf_adj(pkt, ipHdr->version_ihl);
				case (UDP_PROTO_ID):
					udp_packet_parser(pkt,remote_ip);
				break;
				case (TCP_PROTO_ID):
					tcp_packet_parser(pkt);
				break;
				default:
					rte_pktmbuf_free(pkt);
				break;
			}
			else
			{
				rte_pktmbuf_free(pkt);	
			}
		}
	}
    else
    {
		rte_pktmbuf_free(pkt);
	}		
	
}

