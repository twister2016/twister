#ifndef _LCORECONF_H_
#define _LCORECONF_H_

#include <rte_per_lcore.h>
#include <portconf.h>


#define MAX_LCORES 16
#define MAX_PORTS_PER_LCORE 8
#define MAX_TX_PKT_BURST 32

struct lcore_conf {
	uint8_t socket_id;
	uint8_t num_ports;
	uint8_t managed_port[MAX_ETH_PORTS];
	struct mbuf_table tx_mbufs[MAX_ETH_PORTS];
};

struct lcore_conf lcore_conf[MAX_LCORES];

int lcore_conf_init(void);
int port_to_lcore_mapping(void);
int assign_port_to_core(uint8_t, uint8_t);

#endif
