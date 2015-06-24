
#include <rx.h>


int get_pkt_from_rx_queue(struct rte_mbuf ** m, uint8_t port) {
              
	printf("get_pkt_from_rx_queue %d max pkt burst\n", MAX_PKT_BURST);
	unsigned nb_pkt_rx = 0;
	nb_pkt_rx = rte_eth_rx_burst(port, 0, m, MAX_PKT_BURST);
	printf("%d num pkt rx\n", nb_pkt_rx);
	global_stats.packet_received += nb_pkt_rx; //global variable in stats.h
	return nb_pkt_rx;
}

