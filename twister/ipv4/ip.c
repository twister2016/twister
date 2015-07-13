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

#define LOCAL_HOST_IP 213


int ip4_packet_parser(struct rte_mbuf *pkt, uint8_t port_id)
{
	printf("ip4_packet_parser\n");
	struct ipv4_hdr *ipHdr = rte_pktmbuf_mtod(pkt, struct ipv4_hdr *);
    //uint16_t ipchecksum = rte_ipv4_cksum(ipHdr);	
 /*   if (ipchecksum == ipHdr->hdr_checksum && ipHdr->version_ihl > 20 && ipHdr->time_to_live >= 0)
	{*/
	if(CHECK_IPv4_CKSUM) {
		uint16_t ipchecksum = rte_ipv4_cksum(ipHdr);
		if (ipchecksum != ipHdr->hdr_checksum) {
			printf("Incorrect Checksum\n");
			rte_pktmbuf_free(pkt);
			return -1;
		}
	}
	uint32_t dst_ip  = rte_be_to_cpu_32(ipHdr->dst_addr);
	uint32_t src_ip = rte_be_to_cpu_32(ipHdr->src_addr);
	printf("Num ip addrs %d, start ip %d, dst ip %d, range %d\n", port_info[port_id].num_ip_addrs, port_info[port_id].start_ip_addr, dst_ip, port_info[port_id].start_ip_addr + port_info[port_id].num_ip_addrs);
	if (((dst_ip >= port_info[port_id].start_ip_addr) && (dst_ip <= (port_info[port_id].start_ip_addr + port_info[port_id].num_ip_addrs))) || dst_ip == LOCAL_HOST_IP) 	//--!TODO Add IP range logic
	{	
		rte_pktmbuf_adj(pkt, ipHdr->version_ihl);
		printf("IP proto %d\n", ipHdr->next_proto_id);
		switch(ipHdr->next_proto_id)
		{
			case (UDP_PROTO_ID):
			rte_pktmbuf_adj(pkt, sizeof(struct ipv4_hdr));
			if(event_flags_global == GET_L4_PKTS)
				printf("L4 PACKET Received /n");
			//user function should come here
			else
			{
				udp_packet_parser(pkt,src_ip,dst_ip);	//--!TODO implement ipv6
			}
			break;
			case (TCP_PROTO_ID):
				//tcp_packet_parser(pkt);
				break;
			default:
				rte_pktmbuf_free(pkt);
				break;
		}
	}
	else
	{
		printf("IP not matched. Freeing packet\n");
		rte_pktmbuf_free(pkt);	
	}
		
/*	}
    else
    {
		rte_pktmbuf_free(pkt);
	}		
	*/
	return 0;
}



void ip4_packet_create(struct rte_mbuf *pkt,uint8_t next_proto_id,uint32_t src_ip,uint32_t dst_ip,uint16_t length)
{

	rte_pktmbuf_prepend(pkt, sizeof(struct ipv4_hdr));
	struct ipv4_hdr *ip_hdr  = rte_pktmbuf_mtod(pkt, struct ipv4_hdr *);
	ip_hdr->total_length = rte_cpu_to_be_16(length + sizeof(struct ipv4_hdr));
	ip_hdr->next_proto_id = next_proto_id;
	ip_hdr->src_addr = rte_cpu_to_be_32(src_ip);
	ip_hdr->dst_addr = rte_cpu_to_be_32(dst_ip);
	ip_hdr->version_ihl = 0x45;
	ip_hdr->time_to_live = 63;
	ip_hdr->hdr_checksum =rte_ipv4_cksum(ip_hdr);
	int port_id = get_port_by_ip(src_ip);
	if(port_id < 0) {
		printf("Port not found\n");
		return;
	}
	eth_pkt_ctor(pkt, port_id, ETHER_TYPE_IPv4, dst_ip );
}

#endif
