/** @file Allows packets without ARP data to be queued,
 * sends ARP requests for them and when replies are received
 * it starts processing them .
 *
 */

#ifndef _QUEUED_PKTS_H_
#define _QUEUED_PKTS_H_

#include <rte_mbuf.h>
#include <rte_memory.h>
#include <timerfuncs.h>
#include <arplogic.h>
#include <initfuncs.h>

#define MAX_QUEUED_PKTS 1024

uint64_t queued_pkt_time_limit;		//--!TODO get the value from user
uint64_t queue_update_limit;

/** Structure for queued packets.
 *
 */
struct queued_pkt
{
        struct rte_mbuf * pkt;
        uint64_t timercycle;
        uint32_t arp_ip;	//IP for which ARP is needed
        uint16_t port_id;
        struct queued_pkt * next;
}__attribute__((__packed__));

struct queued_pkt * root_queued_pkt;

/** Adds packets without ARP data to queue.
 *
 * @param pkt :Pointer to the packet
 *
 * @param arp_ip_of_pkt :IP to ARP for
 *
 * @param port_id :ID of port through which to send
 *
 * @return 0(TRUE), -1(ERROR)
 */
int tw_add_pkt_to_queue(struct rte_mbuf * pkt, uint32_t arp_ip_of_pkt, uint16_t port_id);

/** Runs after specified number of cycles to update queue
 *
 * @param curr_time_cycle :Current cycle
 *
 * @return 0(TRUE)
 */
int tw_update_queued_pkts(uint64_t curr_timer_cycle);

/** Deletes queued packet
 *
 * @param prev_queued_pkt
 *
 * @param curr_queued_pkt
 *
 * @return 0(TRUE)
 */
int tw_delete_queued_pkt(struct queued_pkt ** prev_queued_pkt, struct queued_pkt ** curr_queued_pkt);

/** Forwards packets with ARP data.
 *
 * @param prev_queued_pkt
 *
 * @param curr_queued_pkt
 *
 * @param eth_mac
 *
 * @return 0(TRUE)
 */
int tw_send_queued_pkt(struct queued_pkt ** prev_queued_pkt, struct queued_pkt ** curr_queued_pkt
		, struct ether_addr eth_mac);

/** Prints all packets in queue
 *
 */
void tw_print_queued_pkts(void);

#endif
