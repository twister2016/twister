#include <rte_ether.h>
#include <vlan.h>
#include <arplogic.h>
#include <event_loop.h>

int tw_vlan_parser(struct rte_mbuf * pkt, uint8_t port_id) {
	rte_vlan_strip(pkt);			//--? should the vlan be compared with the vlan value in port_info[port_id]???
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
	switch(eth->ether_type) {
		case ETHER_TYPE_ARP:
			tw_parse_arp(pkt, port_id);
			break;
		default:
			rte_pktmbuf_free(pkt);
			break;
	}
	return 0;
}


