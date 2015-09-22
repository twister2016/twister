#ifndef _ETH_H_
#define _ETH_H_

#include <inttypes.h>
#include <rte_ether.h>
#include <mempoolconf.h>

extern uint8_t eth_hdr_size;

inline int tw_match_port_ether_addr(struct ether_addr *, char *);
inline void tw_copy_ether_addr(struct ether_addr *, struct ether_addr *);
extern int tw_eth_pkt_ctor(struct rte_mbuf*, uint8_t, uint16_t, uint32_t);
extern int tw_eth_pkt_parser(struct rte_mbuf *, uint8_t);
int tw_send_pkt(tw_buf_t *, char *);
struct ether_addr * tw_get_ether_addr(char *);
#endif
