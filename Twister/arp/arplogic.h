#ifndef _ARP_H_
#define _ARP_H_

#include <rte_arp.h>
#include <rte_malloc.h>
#include <rte_ether.h>
#include "tx.h"
#include <portconf.h>

#define ARP_ENTRY_TIMEOUT 15000

struct arp_table {
	uint32_t ip_addr;
	struct ether_addr eth_mac;
	uint8_t port_id;
	struct arp_table * next;
} __attribute__((__packed__));

struct arp_table * arp_table_root = NULL;
uint32_t arp_table_size = 0;
/******************************************************************************************/
int arp_parser(struct ether_hdr *, uint8_t);
int send_arp_reply(struct ether_hdr *, uint8_t);
int process_arp_reply(struct ether_hdr *, uint8_t);
struct arp_table * search_arp_table(uint32_t);
int add_arp_entry(uint32_t, struct ether_addr, uint8_t);
/******************************************************************************************/

int arp_parser(struct ether_hdr * eth, uint8_t port_id) {
	struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth+1);	//remove the eth header, and see if its a request or reply and act accordingly
	if(arp_pkt->arp_op == ARP_OP_REQUEST) {
		if((arp_pkt->arp_data.arp_tip == port_info->ip_addr) && (port_info[port_id].flags & REPLY_ARP)) {
				send_arp_reply(eth, port_id);
		}
	}
	else if(arp_pkt->arp_op == ARP_OP_REPLY) {
		process_arp_reply(eth, port_id);
	}
	else
		rte_pktmbuf_free((struct rte_mbuf *) eth);
	return 0;
}
	
int send_arp_reply(struct ether_hdr * eth, uint8_t port_id) {
	struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth+1);		//ether_hdr pointer type was passed because we want to reuse the pktmbuf for transmission also
	arp_pkt->arp_op = ARP_OP_REPLY;
	ether_addr_copy(&(arp_pkt->arp_data.arp_sha), &(arp_pkt->arp_data.arp_tha));
	ether_addr_copy(port_info[port_id].eth_mac, &(arp_pkt->arp_data.arp_sha));
	arp_pkt->arp_data.arp_sip = arp_pkt->arp_data.arp_tip;
	arp_pkt->arp_data.arp_tip = port_info[port_id].ip_addr;
	add_pkt_to_tx_queue((struct rte_mbuf *) eth, port_id);				//--!TODO implement add_pkt_to_tx_queue
	return 0;
}	

int process_arp_reply(struct ether_hdr * eth, uint8_t port_id) {
	struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth+1);
	if(search_arp_table(arp_pkt->arp_data.arp_sip) == NULL)	{	//if ARP entry doesn't exist. One MAC can have multiple IPs associated
		if(add_arp_entry(arp_pkt->arp_data.arp_sip, arp_pkt->arp_data.arp_sha, port_id))
			return -1;
	}
	rte_pktmbuf_free((struct rte_mbuf *) eth);
	return 0;
}

struct arp_table * search_arp_table(uint32_t ip_to_search) {
	struct arp_table * arp_table_ptr = arp_table_root;
	while(arp_table_ptr != NULL) {
		if(arp_table_ptr->ip_addr == ip_to_search)
			return arp_table_ptr;
		else	
			arp_table_ptr = arp_table_ptr->next;
	}
	return NULL;
}

int add_arp_entry(uint32_t ip_to_add, struct ether_addr mac_to_add, uint8_t port_id) {
	struct arp_table * arp_table_ptr = arp_table_root;
	while(arp_table_ptr != NULL)
			arp_table_ptr = arp_table_ptr->next;
	arp_table_ptr = rte_malloc("struct arp_table", sizeof(struct arp_table), RTE_CACHE_LINE_SIZE);
	if(arp_table_ptr == NULL)
		return -1;
	arp_table_ptr->ip_addr = ip_to_add;
	ether_addr_copy(&(mac_to_add), &(arp_table_ptr->eth_mac));
	arp_table_ptr->port_id = port_id;
	arp_table_size++;
	return 0;
}

#endif
