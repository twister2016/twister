#ifndef _PKTPARSER_C_
#define _PKTPARSER_C_

#include <pktparser.h>

int parse_pkt(struct rte_mbuf * pkt, uint8_t port_id) {
	eth_pkt_parser(pkt, port_id);
	return 0;
}
 
#endif
