#ifndef _ETHERNET_C_
#define _ETHERNET_C_

#include <rte_ether.h>
#include <portconf.h>
#include <vlan.h>
#include <eth.h>
#include <rte_ether.h>
#include <arplogic.h>
#include <ip.h>
#include <queued_pkts.h>
#include <vlan.h>
#include <event_loop.h>

//static struct ether_addr eth_port_mac[MAX_ETH_PORTS];
int eth_pkt_ctor(struct rte_mbuf* m, uint8_t port_id, uint16_t eth_type, uint32_t dst_ip ) {
	
	//uint8_t socket_id = rte_eth_dev_socket_id(port_id);
	//struct rte_mbuf * m = rte_pktmbuf_alloc ( tx_mempool[socket_id] );
	rte_pktmbuf_prepend(m, sizeof ( struct ether_hdr ));
	struct ether_hdr* eth = rte_pktmbuf_mtod(m, struct ether_hdr *);
	eth->ether_type = rte_cpu_to_be_16(eth_type);
	ether_addr_copy(port_info[port_id].eth_mac, &(eth->s_addr));
	if ( eth_type == ETHER_TYPE_VLAN ) {
        	vlan_ctor(m, port_id, ETHER_TYPE_IPv4); //TODO make it generic
    	}
	uint32_t arp_ip = 0;
    	uint32_t source_and = port_info[port_id].start_ip_addr & port_info[port_id].subnet_mask;
    	uint32_t dest_and = dst_ip & port_info[port_id].subnet_mask;

	if(source_and == dest_and)
		arp_ip = dst_ip;
	else 
		arp_ip = port_info[port_id].gateway_ip;
    	struct arp_table *  arp_table_ptr = search_arp_table(rte_cpu_to_be_32(arp_ip));
        if(arp_table_ptr == NULL) {
		if(add_pkt_to_queue(m, arp_ip, port_id) == 0)
         		construct_arp_packet(arp_ip, port_id); 
        }
        else {
            	ether_addr_copy(&(arp_table_ptr->eth_mac), &(eth->d_addr));
		if(PIPELINE==1)
		{
			add_packet_to_tx_pipeline(m, port_id);
			return 0;
		}
            	add_pkt_to_tx_queue(m, port_id);
        }
    	return 0;
}

int eth_pkt_parser(struct rte_mbuf * pkt, uint8_t port_id, uint8_t processing_flag, void * cb_func) {
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
	uint8_t accept_brdcast = is_broadcast_ether_addr(&(eth->d_addr)) & ACCEPT_BRDCAST;
	if(is_same_ether_addr(&(eth->d_addr), port_info[port_id].eth_mac) || accept_brdcast) {
		switch(rte_be_to_cpu_16(eth->ether_type)) {
		case ETHER_TYPE_ARP:
			arp_parser(pkt, port_id);
			break;
		case ETHER_TYPE_VLAN:
			vlan_parser(pkt, port_id, processing_flag, cb_func);
			break;
		case ETHER_TYPE_IPv4:
			//rte_pktmbuf_adj(pkt, sizeof(struct ether_hdr));
			if(processing_flag == LOOP_PROCESS) {
				ip4_packet_parser(pkt, port_id, sizeof(struct ether_hdr), processing_flag, NULL);	//--!TODO implement ipv6
			}
			else{
				void (* eth_cb_func) (struct rte_mbuf *, uint8_t) = cb_func;
				eth_cb_func(pkt, port_id);
			}
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

#endif
