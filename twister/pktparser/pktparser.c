#ifndef _PKTPARSER_C_
#define _PKTPARSER_C_

#include <pktparser.h>

int parse_pkt(__attribute__((unused)) struct rte_mbuf * pkt, __attribute__((unused)) uint8_t port_id) {   //TODO Use seperate protocol parsing
	//eth_pkt_parser(pkt, port_id);
	return 0;
}
 
#endif
