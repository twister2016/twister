#ifndef _RX_H_
#define _RX_H_

#include <stdint.h>
#include <rte_config.h>
#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_ethdev.h>
#include <lcoreconf.h>
#include <stats.h>

int tw_get_pkt_from_rx_queue(struct mbuf_table * m, uint8_t port, uint8_t queue_id);
int tw_rx_for_each_queue(struct mbuf_table *);


#endif
