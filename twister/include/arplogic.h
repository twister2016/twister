/** @file Contains ARP functionality
 *
 */

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

struct ether_addr broadcastmac;
struct arp_table * arp_table_root;
uint32_t arp_table_size;

/** Parses any ARP packet received
 *
 * @param rte_mbuf :Pointer to memory buffer holding the msg
 *
 * @param uint8_t : ID of port
 *
 * @return 0(TRUE)
 */
int tw_parse_arp(struct rte_mbuf * pkt, uint8_t port_id);

/** Sends reply to an ARP request
 *
 * @param pkt : Pointer to ARP request packet
 *
 * @param port_id :ID of port
 *
 * @return 0(TRUE)
 *
 */
int tw_send_arp_reply(struct rte_mbuf * pkt, uint8_t port_id);

/** Process the ARP reply.
 *
 * @param pkt : Pointer to the ARP reply packet
 *
 * @param port_id :ID of port
 *
 * @return 0(TRUE)
 *
 */
int tw_process_arp_reply(struct rte_mbuf* pkt, uint8_t port_id);

/** Add a new ARP entry to the table
 *
 * @param ip_to_add :New IP to add to table
 *
 * @param mac_to_add :New MAC address to add
 *
 * @return 0(TRUE)
 *
 */
int tw_add_arp_entry(uint32_t ip_to_add, struct ether_addr mac_to_add, uint8_t port_id);

#endif
