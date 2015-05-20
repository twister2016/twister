#ifndef _ETH_H_
#define _ETH_H_

#include <rte_ether.h>
#unclude "portconf.h"


//static struct ether_addr eth_port_mac[MAX_ETH_PORTS];

void eth_pkt_ctor() {			//--!TODO
	return;
}

uint8_t	eth_pkt_parser(rte_mbuf * pkt, uint8_t port_id) {
	struct ether_hdr * eth = rte_pktmbuf_mtod(struct ether_hdr *, pkt);
	uint8_t accept_brdcast = is_broadcast_ether_addr(eth->d_addr) & ACCEPT_BRDCAST;
	if(is_same_ether_addr(eth->d_addr, port_info[port_id].eth_mac) || (is_broadcast_ether_addr(eth->d_addr) & ACCEPT_BRDCAST)) {
		switch(eth->ether_type) {
		case ETHER_TYPE_ARP:
			arp_parser(eth, port_id);
			break;
		case ETHER_TYPE_VLAN:
			vlan_parser(pkt, port_id);
			break;
		case ETHER_TYPE_IPv4:
			ipv4_parser(pkt, port_id);	//--!TODO implement ipv6
			break;
		default:
			rte_pktmbuf_free(pkt);
			break;
		}
	}
	else
		rte_pktmbuf_free(pkt);
}

#endif