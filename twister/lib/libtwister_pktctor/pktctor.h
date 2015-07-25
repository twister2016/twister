#ifndef _PKTCTOR_H_
#define _PKTCTOR_H_
/**
 * @file
 *
 * Packet constructor Helpers in Twister
 */
extern int pkt_ctor(struct rte_mbuf*, void*,uint32_t); /**< pkt_ctor - This function constructs the packet after receiving the packet from the application and calls the callback of the next layer (UDP LAYER). */
#endif
