#ifndef _ARPLOGIC_H_
#define _ARPLOGIC_H_

#include <rte_arp.h>
#include <rte_malloc.h>
#include <rte_ether.h>
#include <tx.h>
#include <portconf.h>
#include <tw_api.h>

#define ARP_ENTRY_TIMEOUT 15000
#define NUM_ARP_RETRIES 3

struct arp_table {
	uint32_t ip_addr;
	struct ether_addr eth_mac;
	uint8_t port_id;
	struct arp_table * next;
} __attribute__((__packed__));

extern struct ether_addr broadcastmac; 
extern struct arp_table * arp_table_root;
extern uint32_t arp_table_size;

int tw_parse_arp(struct rte_mbuf *, uint8_t);
int tw_send_arp_reply(struct rte_mbuf *, uint8_t);
int tw_process_arp_reply(struct ether_hdr *, uint8_t);
//struct arp_table * tw_search_arp_table(uint32_t);
struct ether_addr * tw_search_arp_entry(char *);
int tw_add_arp_entry(uint32_t ip_to_add, struct ether_addr, uint8_t);
//int tw_construct_arp_packet(uint32_t, uint8_t);
int tw_send_arp_request(char *, char *);
void tw_print_arp_table(void);
//int tw_arp_parser(tw_buf_t *, char *);
#endif
