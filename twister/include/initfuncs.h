#ifndef _INITFUNCS_H_
#define _INITFUNCS_H_

#include <getopt.h>
#include <rte_config.h>
#include <rte_eal.h>
#include <lcoreconf.h>
#include <mempoolconf.h>
#include <periodic_timers.h>
#include <queued_pkts.h>
#include <jsonparser.h>
#include <portconf.h>


struct app_params {
	/* CPU cores */
	uint32_t core_rx;
	uint32_t core_worker;
	uint32_t core_tx;

	/* Ports*/
	uint32_t ports[MAX_ETH_PORTS];
	uint32_t n_ports;
	uint32_t port_rx_ring_size;
	uint32_t port_tx_ring_size;

	/* Rings */
	struct rte_ring *rings_rx[MAX_ETH_PORTS];
	struct rte_ring *rings_tx[MAX_ETH_PORTS];
	uint32_t ring_rx_size;
	uint32_t ring_tx_size;

	/* Internal buffers */
	struct mbuf_table mbuf_rx;
	struct mbuf_table mbuf_tx[MAX_ETH_PORTS];

	/* Buffer pool */
	struct rte_mempool *pool;
	uint32_t pool_buffer_size;
	uint32_t pool_size;
	uint32_t pool_cache_size;

	/* Burst sizes */
	uint32_t burst_size_rx_read;
	uint32_t burst_size_rx_write;
	uint32_t burst_size_worker_read;
	uint32_t burst_size_worker_write;
	uint32_t burst_size_tx_read;
	uint32_t burst_size_tx_write;

	/* App behavior */
	uint32_t pipeline_type;
} __rte_cache_aligned;

extern struct app_params app;

extern int init_eal_env(int, char **);
extern int parse_twister_args(int argc, char **argv);
extern int tw_init_global(int argc, char **);
extern int init_user_given_vals(void);
int display_usage(const char *);
int parse_portmask(const char *);

#endif
