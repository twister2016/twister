#ifndef _ETH_H_
#define _ETH_H_
/**
 * @file
 *
 * Ethernet Helpers in Twister
 */
#include <inttypes.h>
#include <rte_ether.h>

extern int eth_pkt_ctor(struct rte_mbuf*, uint8_t, uint16_t, uint32_t); /**< eth_pkt_ctor - This functions construct the Ethernet header and attach it to the packet and forward the packet to respective queue to be send. */
extern int eth_pkt_parser(struct rte_mbuf *, uint8_t); /**< eth_pkt_parser - This functions parse the Ethernet header and forward the packet to respective next callback. */
extern uint8_t eth_hdr_size; /**< eth_hdr_size - This variable contains the size information of the Ethernet Header. */
#endif
