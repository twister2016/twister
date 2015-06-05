
#include <rx.h>


int get_pkt_from_rx_queue(struct rte_mbuf ** m, uint8_t port) {
              
	unsigned nb_pkt_rx = 0;
	nb_pkt_rx = rte_eth_rx_burst(port, 0,m, MAX_PKT_BURST);
	return nb_pkt_rx;
	
}

