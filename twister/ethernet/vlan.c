#include <rte_ether.h>
//#include <vlan.h>
#include <arplogic.h>
#include <event_loop.h>

#include <tw_common.h>

inline int tw_vlan_strip(struct tw_buf_s *buffer) {
    rte_vlan_strip(buffer->pkt);
    return 0;
}
/*
int tw_vlan_parser(tw_buf_t * pkt, uint8_t port_id) {
	tw_vlan_strip(pkt);			//--? should the vlan be compared with the vlan value in port_info[port_id]???
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt->pkt, struct ether_hdr *);
	switch(eth->ether_type) {
		case ETHER_TYPE_ARP:
			tw_parse_arp(pkt->pkt, port_id);
			break;
		default:
			rte_pktmbuf_free(pkt->pkt);
			break;
	}
	return 0;
}*/


