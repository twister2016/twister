#ifndef _ARPLOGIC_H_
#define _ARPLOGIC_H_
/**
 * @file
 *
 * ARP Helpers in Twister
 */

#include <rte_arp.h>
#include <rte_malloc.h>
#include <rte_ether.h>
#include <tx.h>
#include <portconf.h>

#define ARP_ENTRY_TIMEOUT 15000
#define NUM_ARP_RETRIES 3

struct arp_table {
	uint32_t ip_addr;  /**< IP Address - ARP TABLE Field*. */
	struct ether_addr eth_mac; /**< Ethernet MAC Address - ARP TABLE Field*. */
	uint8_t port_id; /**< Port ID - ARP TABLE Field*. */
	struct arp_table * next; /**< Next - To point to the next ARP TABLE ENTRY*. */
} __attribute__((__packed__));

extern struct ether_addr broadcastmac; 
extern struct arp_table * arp_table_root;
extern uint32_t arp_table_size;

int arp_parser(struct rte_mbuf *, uint8_t);
int send_arp_reply(struct rte_mbuf *, uint8_t);
int process_arp_reply(struct ether_hdr *, uint8_t);
struct arp_table * search_arp_table(uint32_t);
int add_arp_entry(uint32_t ip_to_add, struct ether_addr, uint8_t);
int construct_arp_packet(uint32_t, uint8_t);
void print_arp_table(void);
#endif
