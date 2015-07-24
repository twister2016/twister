#ifndef _PKTCTOR_H_
#define _PKTCTOR_H_
/**
 * @file
 *
 * Packet constructor Helpers in Twister
 */
extern int pkt_ctor(struct rte_mbuf*, void*,uint32_t); 
//extern int pkt_parser(struct rte_mbuf *, uint8_t);
#endif
