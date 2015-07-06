#ifndef _LCORECONF_H_
#define _LCORECONF_H_

#include <rte_per_lcore.h>
#include <portconf.h>

#define MAX_LCORES 16
#define MAX_RX_QUEUES_PER_LCORE 8
#define MAX_TX_PKT_BURST 32

struct lcore_port_queue {
	uint8_t port_id;
	uint8_t queue_id;
};

struct lcore_conf {
	uint8_t socket_id;
	uint8_t num_queues;
	uint8_t core_rx;
	uint8_t core_tx;
	uint8_t num_port;
	uint8_t managed_port[MAX_RX_QUEUES_PER_LCORE];
	struct lcore_port_queue mngd_queues[MAX_RX_QUEUES_PER_LCORE];
	struct mbuf_table tx_mbufs[MAX_ETH_PORTS];
} __rte_cache_aligned;

struct lcore_conf lcore_conf[MAX_LCORES];

int lcore_conf_init(void);


#endif
