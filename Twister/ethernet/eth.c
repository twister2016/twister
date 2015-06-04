#include <rte_ether.h>
#include <portconf.h>
#include <vlan.h>
#include <eth.h>
#include <arplogic.h>
#include <ip.h>

//static struct ether_addr eth_port_mac[MAX_ETH_PORTS];


/*int eth_pkt_ctor(void) {			//--!TODO
	return 0;
}*/

int eth_pkt_parser(struct rte_mbuf * pkt, uint8_t port_id) {
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
	uint8_t accept_brdcast = is_broadcast_ether_addr(&(eth->d_addr)) & ACCEPT_BRDCAST;
	if(is_same_ether_addr(&(eth->d_addr), port_info[port_id].eth_mac) || accept_brdcast) {
		switch(eth->ether_type) {
		case ETHER_TYPE_ARP:
			arp_parser(eth, port_id);
			break;
		case ETHER_TYPE_VLAN:
			vlan_parser(pkt, port_id);
			break;
		case ETHER_TYPE_IPv4:
			ip4_packet_parser(pkt, port_id);	//--!TODO implement ipv6
			break;
		default:
			rte_pktmbuf_free(pkt);
			break;
		}
	}
	else
		rte_pktmbuf_free(pkt);
	return 0;
}


