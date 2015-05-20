#ifndef _ARP_H_
#def _ARP_H_

#include <rte_arp.h>

#define ARP_ENTRY_TIMEOUT 15000

int	arp_parser(struct ether_hdr * eth, uint8_t port_id);
int	send_arp_reply(struct ether_hdr * eth, uint8_t port_id);
int process_arp_reply(struct ether_hdr * eth, uint8_t port_id);

int	arp_parser(struct ether_hdr * eth, uint8_t port_id){
	struct arp_hdr * arp_pkt = (struct arp_ipv4 *) (eth+1);	//remove the eth header, and see if its a request or reply and act accordingly
	if(arp_pkt->arp_op == ARP_OP_REQUEST)
	{
		if(arp_pkt->arp_data.arp_tip == port_info->ip_addr && port_info[port_id].flags & REPLY_ARP) {
				send_arp_reply(struct ether_hdr * eth, uint8_t port_id);	//--!TODO entry to be added in ARP table
		}
	}
	else if(arp_pkt->arp_op == ARP_OP_REPLY) {
		process_arp_reply(struct ether_hdr * eth, uint8_t port_id);
	}
	else
		rte_pktmbuf_free((rte_mbuf *) eth);
	return 0;
}
	
int	send_arp_reply(struct ether_hdr * eth, uint8_t port_id) {
	struct arp_hdr * arp_pkt = (struct arp_ipv4 *) (eth+1);		//ether_hdr pointer type was passed because we want to reuse the pktmbuf for transmission also
	arp_pkt->arp_op = ARP_OP_REPLY;
	ether_addr_copy(&(arp_pkt->arp_data.arp_sha), &(arp_pkt->arp_data.arp_tha));
	ether_addr_copy(port_info[port_id].eth_mac, &(arp_pkt->arp_data.arp_sha));
	arp_pkt->arp_data.arp_sip = arp_pkt->arp_data.arp_tip;
	arp_pkt->arp_data.arp_tip = port_info[port_id].ip_addr;
	add_pkt_to_tx_queue((rte_mbuf *) eth, port_id);
	return 0;
}	

int process_arp_reply(struct ether_hdr * eth, uint8_t port_id) {
	//add_arp_entry(struct ether_hdr * eth, uint8_t port_id);			//--!should this be one function or a separate call???
	return 0;			
}

#endif
