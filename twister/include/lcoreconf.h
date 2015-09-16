#ifndef _LCORECONF_H_
#define _LCORECONF_H_

#include <rte_per_lcore.h>
#include <portconf.h>

#define MAX_LCORES 16
#define MAX_RX_QUEUES_PER_LCORE 8
#define MAX_TX_PKT_BURST 32
#define MASTER_LCORE 0

/*
struct lcore_port_queue {
	uint8_t port_id;
	uint8_t queue_id;
};
*/

struct lcore_conf {
	char core_name[10];
	uint8_t core_id_external;
	uint8_t socket_id;
	uint8_t core_rx;
	uint8_t core_tx;
	uint8_t core_working;
	uint8_t num_port;
	uint8_t managed_port[MAX_ETH_PORTS];
	//struct lcore_port_queue mngd_queues[MAX_RX_QUEUES_PER_LCORE];
	struct mbuf_table tx_mbufs[MAX_ETH_PORTS];
} __rte_cache_aligned;

struct lcore_conf lcore_conf[MAX_LCORES];

int tw_lcore_conf_init(void);
int tw_engine_name_to_lcore_id(char*);
int tw_lcore_pipeline_init(void);
int tw_map_port_to_engine(char*, char*);



#endif
