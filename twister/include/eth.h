#ifndef _ETH_H_
#define _ETH_H_

#include <inttypes.h>
#include <rte_ether.h>

extern int eth_pkt_ctor(struct rte_mbuf*, uint8_t, uint16_t, uint32_t);
extern int eth_pkt_parser(struct rte_mbuf *, uint8_t);
uint8_t eth_hdr_size=20;
#endif
