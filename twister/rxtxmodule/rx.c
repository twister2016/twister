#include <rx.h>
#include <event_loop.h>

int rx_for_each_queue(struct mbuf_table * m) {
	unsigned lcore_id,nb_pkt_rx=0;
	struct lcore_conf *qconf;
	lcore_id = rte_lcore_id();
	qconf = &lcore_conf[lcore_id];
	int i=0;
	for(i=0;i<qconf->num_queues;i++)
	{
		nb_pkt_rx += get_pkt_from_rx_queue(m + nb_pkt_rx ,qconf->mngd_queues[i].port_id,qconf->mngd_queues[i].queue_id);
	}
	return nb_pkt_rx;
}

int get_pkt_from_rx_queue(struct mbuf_table * m, uint8_t port,uint8_t queue_id) {
              
	//printf("get_pkt_from_rx_queue %d max pkt burst\n", MAX_PKT_BURST);
	unsigned nb_pkt_rx = 0;
	nb_pkt_rx = rte_eth_rx_burst(port, queue_id, m->m_table, MAX_PKT_BURST);
	//printf("%d num pkt rx\n", nb_pkt_rx);
	global_stats.packet_received += nb_pkt_rx; //global variable in stats.h
	return nb_pkt_rx;
}

