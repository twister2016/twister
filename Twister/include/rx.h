#ifndef _RX_H_
#define _RX_H_

#include <stdint.h>
#include <rte_config.h>
#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_ethdev.h>
#include <lcoreconf.h>

int get_pkt_from_rx_queue(struct rte_mbuf ** m, uint8_t port);


#endif
