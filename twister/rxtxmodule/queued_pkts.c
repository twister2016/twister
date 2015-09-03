#ifndef _QUEUED_PKTS_C_
#define _QUEUED_PKTS_C_

#include <queued_pkts.h>

uint64_t queued_pkt_time_limit = 10;  //keep a pkt in queue for queued_pkt_time_limit sec
uint64_t queue_update_limit = 10; //update queued pkts every queue_update_limit millisecs
uint32_t total_queued_pkts = 0;
struct queued_pkt * root_queued_pkt = NULL;

int add_pkt_to_queue(struct rte_mbuf * pkt, uint32_t arp_ip_of_pkt, uint16_t port_id) {
	if(total_queued_pkts >= MAX_QUEUED_PKTS) {
		rte_pktmbuf_free(pkt);
		return -1;
	}
		
	uint64_t curr_timer_cycle = tw_get_current_timer_cycles();
	struct queued_pkt * pkt_to_queue = root_queued_pkt;
	if(pkt_to_queue == NULL) {
		pkt_to_queue = rte_malloc("struct queued_pkt", sizeof(struct queued_pkt), RTE_CACHE_LINE_SIZE);
		root_queued_pkt = pkt_to_queue;
	}
	else {
		while(pkt_to_queue->next != NULL)
			pkt_to_queue = pkt_to_queue->next;
		pkt_to_queue->next = rte_malloc("struct queued_pkt", sizeof(struct queued_pkt), RTE_CACHE_LINE_SIZE);
		pkt_to_queue = pkt_to_queue->next;
	}
	if (pkt_to_queue == NULL)
		return -1;
	pkt_to_queue->pkt = pkt;
	pkt_to_queue->timercycle = curr_timer_cycle;
	pkt_to_queue->arp_ip = rte_cpu_to_be_32(arp_ip_of_pkt);
	pkt_to_queue->port_id = port_id;
	pkt_to_queue->next = NULL;
	total_queued_pkts++;
	return 0;
}

/*
 * This function will update the queued packets. If timestamp has expired, the packet will be deleted.
 * If ARP reply for queued packet has arrived and added in ARP table, the packet will be sent.
 */
int update_queued_pkts(uint64_t curr_timer_cycle) { 
	uint64_t time_diff;
	struct queued_pkt * curr_queued_pkt = root_queued_pkt;
	struct queued_pkt * prev_queued_pkt = curr_queued_pkt;
	while(curr_queued_pkt) {
		time_diff = get_time_diff(curr_timer_cycle, curr_queued_pkt->timercycle, one_sec);
		if(likely(time_diff < queued_pkt_time_limit)) {
			struct arp_table * arp_entry = search_arp_table(curr_queued_pkt->arp_ip);
			if(unlikely(arp_entry != NULL))
				send_queued_pkt(&prev_queued_pkt, &curr_queued_pkt, arp_entry->eth_mac);
			else
				curr_queued_pkt = curr_queued_pkt->next;
			continue;
		}
		rte_pktmbuf_free(curr_queued_pkt->pkt);			//Else delete the queued packet as time limit has passed
		delete_queued_pkt(&prev_queued_pkt, &curr_queued_pkt);
	}
	return 0;
}

int delete_queued_pkt(struct queued_pkt ** prev_queued_pkt, struct queued_pkt ** curr_queued_pkt) {
	struct queued_pkt * temp_queued_pkt = (*curr_queued_pkt)->next;
	if(*curr_queued_pkt == root_queued_pkt) {
		root_queued_pkt = (*curr_queued_pkt)->next;
		*prev_queued_pkt = root_queued_pkt;
	}
	else {
		(*prev_queued_pkt)->next = (*curr_queued_pkt)->next;
	}
	rte_free((*curr_queued_pkt));
	(*curr_queued_pkt) = temp_queued_pkt;
	total_queued_pkts--;
	return 0;
}

int send_queued_pkt(struct queued_pkt ** prev_queued_pkt, struct queued_pkt ** curr_queued_pkt, struct ether_addr eth_mac) {
	struct rte_mbuf * pkt = (*curr_queued_pkt)->pkt; 
	struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
	ether_addr_copy(&eth_mac, &(eth->d_addr));
	uint8_t port_id = (*curr_queued_pkt)->port_id;	
	delete_queued_pkt(prev_queued_pkt, curr_queued_pkt);
	add_pkt_to_tx_queue(pkt, port_id);
	total_queued_pkts--;
	return 0;
}

void print_queued_pkts(void) {
	printf("printing queued pkts\n");
	struct queued_pkt * temp_queued_pkt = root_queued_pkt;
	while(temp_queued_pkt) {
		printf("queued pkt addr %p\n", temp_queued_pkt);
		temp_queued_pkt = temp_queued_pkt->next;
	}
	printf("\n");
}

#endif
