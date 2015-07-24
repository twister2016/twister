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

int add_pkt_to_queue(struct rte_mbuf *, uint32_t, uint16_t);
int update_queued_pkts(uint64_t);
int delete_queued_pkt(struct queued_pkt **, struct queued_pkt **);
int send_queued_pkt(struct queued_pkt **, struct queued_pkt **, struct ether_addr);
void print_queued_pkts(void);

#endif
