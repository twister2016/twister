#ifndef _QUEUED_PKTS_H_
#define _QUEUED_PKTS_H_
/**
 * @file
 *
 * Queued Packets Helpers in Twister
 */
#include <rte_mbuf.h>
#include <rte_memory.h>
#include <timerfuncs.h>
#include <arplogic.h>
#include <initfuncs.h>
#include <runtime.h>

extern uint64_t queued_pkt_time_limit;		//--!TODO get the value from user
extern uint64_t queue_update_limit;

struct queued_pkt {
	struct rte_mbuf * pkt;
	uint64_t timercycle;
	uint32_t arp_ip;	//IP for which ARP is needed
	uint16_t port_id;
	struct queued_pkt * next;
} __attribute__((__packed__));

extern struct queued_pkt * root_queued_pkt;

int add_pkt_to_queue(struct rte_mbuf *, uint32_t, uint16_t); /**< add_pkt_to_queue - This function when call add the packets to the queue. */
int update_queued_pkts(uint64_t);  /**< update_queued_pkts - This function when call update the number of the packets in the queue. */
int delete_queued_pkt(struct queued_pkt **, struct queued_pkt **); /**< update_queued_pkts - This function when call delete specified queue packet. */
int send_queued_pkt(struct queued_pkt **, struct queued_pkt **, struct ether_addr); /**< send_queued_pkt - This function when call send the specified queue packet that was waiting for the ARP. */
void print_queued_pkts(void); /**< print_queued_pkts - This function when call print all the packets currently residing in the queue. */

#endif
