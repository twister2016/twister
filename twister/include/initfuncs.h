/** @file Contains all functions necessary to initialize Twister environment
 *
 */
#ifndef _INITFUNCS_H_
#define _INITFUNCS_H_
#include <rte_ip.h>
#include <event_loop.h>
#include <getopt.h>
#include <rte_config.h>
#include <rte_eal.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include <lcoreconf.h>
#include <mempoolconf.h>
#include <periodic_timers.h>
#include <queued_pkts.h>
#include <jsonparser.h>
#include <portconf.h>
#include <tw_api.h>

/** Structure to hold application parameters such as cores and ports
 *
 */
struct app_params
{
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
}__rte_cache_aligned;

/** Structure to hold configuration of Twister such as blacklist ports.
 *
 */
struct tw_conf
{
        char * portmask;
        char * coremask;
        char ** blacklist;
        uint32_t blacklist_size;
} twister_config;

struct app_params app;

/** Initializer function used for smoke test
 *
 * @return 0(TRUE), -1(False)
 */
int tw_smoke_init_eal_env(int argc, char ** argv);

/** Initializes environment abstraction layer
 *
 * @return 0(TRUE), -1(FALSE)
 */
int tw_init_eal_env(int argc, char ** argv);

/** Parses application specific parameters
 *
 * @return 0(TRUE), -1(FALSE)
 */
int tw_parse_twister_args(int argc, char **argv);

/** Reads the parameters from conf file
 *
 * @param tw_conf_path :Path of the conf file
 *
 * @return 0(TRUE)
 */
int tw_parse_conf(const char * tw_conf_path);

/** Calls another function which assigns IP to port
 *
 * @return 0(TRUE)
 */
int tw_init_user_given_vals(void);

/** Displays an error if the current configuration is invalid
 *
 * @param prgname :Name of program
 *
 * @return 0(TRUE)
 */
int tw_display_usage(const char * prgname);

/** Parses the hexadecimal port mask to return a decimal value
 *
 * @param portmask : Port mask in hexadecimal
 *
 * @return The port mask in decimal form
 */
int tw_parse_portmask(const char * portmask);

#endif
