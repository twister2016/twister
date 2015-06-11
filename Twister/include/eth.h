#ifndef _ETH_H_
#define _ETH_H_
extern int eth_pkt_ctor(struct rte_mbuf*, uint8_t, uint16_t, uint32_t);
extern int eth_pkt_parser(struct rte_mbuf *, uint8_t);
#endif
