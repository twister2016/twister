#include <rx.h>
#include <event_loop.h>

int tw_rx_for_each_queue(struct mbuf_table * m) {
    
	unsigned lcore_id,nb_pkt_rx=0;
	struct lcore_conf *qconf;
	uint32_t total_pkts_rx = 0;
	lcore_id = rte_lcore_id();
	qconf = &lcore_conf[lcore_id];
        int i =0;
        for (i =0 ; i< qconf->num_port  ;i++ ){
            
            nb_pkt_rx = tw_get_pkt_from_rx_queue(&m[i],qconf->managed_port[i],0);
		    m[i].portid=qconf->managed_port[i];
		    m[i].len=nb_pkt_rx;
		    total_pkts_rx +=nb_pkt_rx;    
    
        }	
	return total_pkts_rx;
}

int tw_get_pkt_from_rx_queue(struct mbuf_table * m, uint8_t port,uint8_t queue_id) {
              
	//printf("get_pkt_from_rx_queue %d max pkt burst\n", MAX_RX_PKT_BURST);
	unsigned nb_pkt_rx = 0;
	nb_pkt_rx = rte_eth_rx_burst(port, queue_id, m->m_table, MAX_RX_PKT_BURST);
	//printf("%d num pkt rx\n", nb_pkt_rx);
	global_stats_option.pkts_rx += nb_pkt_rx; //global variable in stats.h
	return nb_pkt_rx;
}

