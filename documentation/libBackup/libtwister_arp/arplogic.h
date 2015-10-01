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

#define ARP_ENTRY_TIMEOUT 15000 /**< ARP Entry Timeout - This macro defines the timeout after which ARP entry is deleted from the Table. */
#define NUM_ARP_RETRIES 3 /**< Number of ARP Retries - This macro defines the maximum retries for ARP query if ARP reply isn't received. */

struct arp_table {
	uint32_t ip_addr;  /**< IP Address - ARP TABLE Field. */
	struct ether_addr eth_mac; /**< Ethernet MAC Address - ARP TABLE Field. */
	uint8_t port_id; /**< Port ID - ARP TABLE Field. */
	struct arp_table * next; /**< Next - To point to the next ARP TABLE ENTRY. */
} __attribute__((__packed__));

extern struct ether_addr broadcastmac; /**< Broadcast MAC - This structure contains the broadcast MAC. */
extern struct arp_table * arp_table_root;
extern uint32_t arp_table_size; /**< ARP Table Size - Determines the maximum numbers of ARP entries . */

int arp_parser(struct rte_mbuf *, uint8_t); /**< ARP Parser - This Function parse the ARP packet . */
int send_arp_reply(struct rte_mbuf *, uint8_t); /**< Send ARP Reply - This Function send ARP reply, If ARP query is received by the APP. */
int process_arp_reply(struct ether_hdr *, uint8_t); /**< Process ARP Reply - This Function process ARP reply when the APP received the ARP answer. */
struct arp_table * search_arp_table(uint32_t); /**< Search ARP Entry - This Function search the ARP entry w.r.t the IP provided. */
int add_arp_entry(uint32_t ip_to_add, struct ether_addr, uint8_t); /**< Add ARP Entry - This Function add ARP entry to the table. */
int construct_arp_packet(uint32_t, uint8_t); /**< Construct ARP Packet - This Function construct ARP packet and add the generic ARP packet entries. */
void print_arp_table(void); /**< Print ARP Table - This Function simply displays the ARP Table with all the entries in it. */
#endif
