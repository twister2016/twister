#ifndef _ETH_H_
#define _ETH_H_

#include <mempoolconf.h>
#include <inttypes.h>
#include <rte_ether.h>
#include <mempoolconf.h>
#include <event_loop.h>
#include <tw_api.h>
#include <tw_common.h>

uint8_t eth_hdr_size;

int tw_eth_pkt_ctor(struct rte_mbuf*, uint8_t, uint16_t, uint32_t);
int tw_eth_pkt_parser(struct rte_mbuf *, uint8_t);

#endif
