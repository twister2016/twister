#ifndef _RX_H_
#define _RX_H_

#include <rte_mbuf.h>

/*************************************************************/
int add_pkt_to_tx_queue(struct rte_mbuf *, uint8_t);
/*************************************************************/

int add_pkt_to_tx_queue(struct rte_mbuf * eth, uint8_t port_id) {
	return 0;
}



#endif
