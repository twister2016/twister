#ifndef _VLAN_C_
#define _VLAN_C_

#include <rte_ether.h>
#include <vlan.h>
#include <ip.h>
#include <arplogic.h>
#include <event_loop.h>

int vlan_parser(struct rte_mbuf * pkt, uint8_t port_id, uint8_t processing_flag, void * cb_func) {
	rte_vlan_strip(pkt);			//--? should the vlan be compared with the vlan value in port_info[port_id]???
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
	switch(eth->ether_type) {
		case ETHER_TYPE_ARP:
			arp_parser(pkt, port_id);
			break;
		case ETHER_TYPE_IPv4:
			if(processing_flag == LOOP_PROCESS) {
				ip4_packet_parser(pkt, port_id, sizeof(struct ether_hdr), processing_flag, NULL);	//--!TODO implement ipv6
			}
			else{
				void (* vlan_cb_func) (struct rte_mbuf *, uint8_t) = cb_func;
				vlan_cb_func(pkt, port_id);
			}	
			break;
		default:
			rte_pktmbuf_free(pkt);
			break;
	}
	return 0;
}

#endif
