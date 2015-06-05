#ifndef _VLAN_H_
#define _VLAN_H_

#include <rte_ether.h>
#include <ipv4_parser.h>

/**********************************************/
int vlan_parser(struct rte_mbuf *, uint8_t);
/*********************************************/


int vlan_parser(struct rte_mbuf * pkt, uint8_t port_id) {
	rte_vlan_strip(pkt);			//--? should the vlan be compared with the vlan value in port_info[port_id]???
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
	switch(eth->ether_type) {
		case ETHER_TYPE_ARP:
			arp_parser(eth, port_id);
			break;
		/*case ETHER_TYPE_VLAN:					//--!Research...Case of double VLAN...should support or not???
			vlan_parser(pkt, port_id);
			break; */
		case ETHER_TYPE_IPv4:
			ipv4_parser(pkt, port_id);	//--!TODO implement ipv6
			break;
		default:
			rte_pktmbuf_free(pkt);
			break;
	}
	return 0;
}

#endif
