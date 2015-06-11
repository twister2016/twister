#ifndef _PKTPARSER_H_
#define _PKTPARSER_H_

#include <eth.h>

int parse_pkt(struct rte_mbuf * pkt, port_id) {
	eth_pkt_parser(pkt, port_id);
	return 0;
}
 
#endif
