#ifndef _PKTPARSER_H_
#define _PKTPARSER_H_

#include <eth.h>

int tw_parse_pkt(struct rte_mbuf *, uint8_t);
 
#endif
