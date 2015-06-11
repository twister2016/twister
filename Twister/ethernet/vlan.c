#include <rte_ether.h>
#include <vlan.h>
#include <ip.h>
#include <arplogic.h>

/**********************************************/

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
			ip4_packet_parser(pkt, port_id);	//--!TODO implement ipv6
			break;
		default:
			rte_pktmbuf_free(pkt);
			break;
	}
	return 0;
}

int vlan_ctor(struct rte_mbuf * pkt, uint8_t port_id, uint16_t ether_type) {
    
    rte_pktmbuf_prepend(pkt, sizeof (struct vlan_hdr) );
    struct vlan_hdr* vlan_header = rte_pktmbuf_mtod (pkt, struct vlan_hdr *);
    vlan_header->vlan_tci = port_info[port_id].vlan_tag ;
    vlan_header->eth_proto = ether_type; 
    return 0;
}


