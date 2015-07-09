#ifndef _ARPLOGIC_C_
#define _ARPLOGIC_C_

#include <arplogic.h>
#include <initfuncs.h>
#include <runtime.h>

struct arp_table * arp_table_root = NULL;
uint32_t arp_table_size = 0;
struct ether_addr broadcastmac ={
   .addr_bytes[1] = 0xff,
   .addr_bytes[2] = 0xff,
   .addr_bytes[3] = 0xff,
   .addr_bytes[4] = 0xff,
   .addr_bytes[5] = 0xff,
   .addr_bytes[0] = 0xff,
};

struct ether_addr querycastmac ={
   .addr_bytes[1] = 0x00,
   .addr_bytes[2] = 0x00,
   .addr_bytes[3] = 0x00,
   .addr_bytes[4] = 0x00,
   .addr_bytes[5] = 0x00,
   .addr_bytes[0] = 0x00,
};

int arp_parser(struct rte_mbuf * pkt, uint8_t port_id) {
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
	struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth+1);	
	printf("arp_parser\n");
	//remove the eth header, and see if its a request or reply and act accordingly
	if(rte_be_to_cpu_16(arp_pkt->arp_op) == ARP_OP_REQUEST) {
		printf("arp tip %d, port ip %d\n", rte_be_to_cpu_32(arp_pkt->arp_data.arp_tip), port_info[port_id].start_ip_addr);
		if(rte_be_to_cpu_32(arp_pkt->arp_data.arp_tip) == port_info[port_id].start_ip_addr) { //&& (port_info[port_id].flags & REPLY_ARP)) {
				printf("send arp reply\n");
				send_arp_reply(pkt, port_id);
				return 0;
		}
	}
	else if(rte_be_to_cpu_16(arp_pkt->arp_op) == ARP_OP_REPLY) {
		printf("process arp reply\n");
		process_arp_reply(eth, port_id);
		return 0;
	}
	
	printf("delete ARP pkt\n");
	rte_pktmbuf_free(pkt);
	return 0;
}
	
int send_arp_reply(struct rte_mbuf * pkt, uint8_t port_id) {
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
	struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth+1);
	arp_pkt->arp_op = rte_cpu_to_be_16(ARP_OP_REPLY);
	ether_addr_copy(&(arp_pkt->arp_data.arp_sha), &(arp_pkt->arp_data.arp_tha));
	ether_addr_copy(port_info[port_id].eth_mac, &(arp_pkt->arp_data.arp_sha));
	arp_pkt->arp_data.arp_tip = arp_pkt->arp_data.arp_sip;
	arp_pkt->arp_data.arp_sip = rte_cpu_to_be_32(port_info[port_id].start_ip_addr);
	ether_addr_copy(&(eth->s_addr), &(eth->d_addr));
	ether_addr_copy(port_info[port_id].eth_mac, &(eth->s_addr));
	if(PIPELINE==1)
	{
		add_packet_to_tx_pipeline(pkt, port_id);
		return 0;
	}
	add_pkt_to_tx_queue(pkt, port_id);

	return 0;
}	

int process_arp_reply(struct ether_hdr * eth, uint8_t port_id) {
	struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth+1);
	if(search_arp_table(arp_pkt->arp_data.arp_sip) == NULL)	{	
	//if ARP entry doesn't exist. One MAC can have multiple IPs associated
		if(add_arp_entry(arp_pkt->arp_data.arp_sip, arp_pkt->arp_data.arp_sha, port_id)) {
			rte_pktmbuf_free((struct rte_mbuf *) eth);
			return -1;
		}
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
	if(arp_table_ptr == NULL) {
		arp_table_ptr = rte_malloc("struct arp_table", sizeof(struct arp_table), RTE_CACHE_LINE_SIZE);
	}
	else {
		while(arp_table_ptr->next != NULL)
				arp_table_ptr = arp_table_ptr->next;
		arp_table_ptr->next = rte_malloc("struct arp_table", sizeof(struct arp_table), RTE_CACHE_LINE_SIZE);
		arp_table_ptr = arp_table_ptr->next;
	}
	if(arp_table_ptr == NULL)
		rte_exit(EXIT_FAILURE,"CAN'T ALLOCATE ARP TABLE ENTRY\n");

	arp_table_ptr->ip_addr = ip_to_add;
	ether_addr_copy(&(mac_to_add), &(arp_table_ptr->eth_mac));
	arp_table_ptr->port_id = port_id;
	arp_table_ptr->next = NULL;
	arp_table_size++;
	printf("ARP entry added\n");
	return 0;
}

int construct_arp_packet(uint32_t ip, uint8_t port_id) {

	int socket_id = rte_eth_dev_socket_id(port_id);
	if(socket_id == -1)
		socket_id = 0;
	struct rte_mbuf * m = rte_pktmbuf_alloc ( tx_mempool[socket_id] );
	rte_pktmbuf_append(m, sizeof (struct arp_hdr) );
	struct arp_hdr * arp_pkt = rte_pktmbuf_mtod(m, struct arp_hdr *);
    	arp_pkt->arp_op =rte_cpu_to_be_16(ARP_OP_REQUEST);
	arp_pkt->arp_pro=rte_cpu_to_be_16(ETHER_TYPE_IPv4);
	arp_pkt->arp_pln=4;
    	arp_pkt->arp_hln=6;
	arp_pkt->arp_hrd=rte_cpu_to_be_16(ARP_HRD_ETHER);
    	ether_addr_copy(port_info[port_id].eth_mac, &(arp_pkt->arp_data.arp_sha));
	ether_addr_copy(&(querycastmac), &(arp_pkt->arp_data.arp_tha));
	arp_pkt->arp_data.arp_sip = rte_cpu_to_be_32(port_info[port_id].start_ip_addr);
	arp_pkt->arp_data.arp_tip = rte_cpu_to_be_32(ip);
    
    	rte_pktmbuf_prepend( m, sizeof ( struct ether_hdr )  );
    	struct ether_hdr* eth = rte_pktmbuf_mtod(m, struct ether_hdr *);
    	eth->ether_type = rte_cpu_to_be_16(ETHER_TYPE_ARP);
    	ether_addr_copy(port_info[port_id].eth_mac, &(eth->s_addr));
	ether_addr_copy(&(broadcastmac), &(eth->d_addr));
	if(PIPELINE==1)
	{
		add_packet_to_tx_pipeline(m, port_id);
		return 0;
	}
	add_pkt_to_tx_queue(m, port_id);				
				
	return 0;
}

void print_arp_table(void) {
        struct arp_table * temp_arp_entry = arp_table_root;
        while(temp_arp_entry != NULL) {
                printf("IP: %d\n", temp_arp_entry->ip_addr);
                printf("MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                                temp_arp_entry->eth_mac.addr_bytes[0],
                                temp_arp_entry->eth_mac.addr_bytes[1],
                                temp_arp_entry->eth_mac.addr_bytes[2],
                                temp_arp_entry->eth_mac.addr_bytes[3],
                                temp_arp_entry->eth_mac.addr_bytes[4],
                                temp_arp_entry->eth_mac.addr_bytes[5]);
                printf("Port ID: %d\n", temp_arp_entry->port_id);
                temp_arp_entry = temp_arp_entry->next;
        }
        return;
}
	

#endif

