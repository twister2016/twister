#ifndef _LCORECONF_H_
#define _LCORECONF_H_
/**
 * @file
 *
 * Lcore Configuration Helpers in Twister
 */
#include <rte_per_lcore.h>
#include <portconf.h>

#define MAX_LCORES 16 /**< MAX_LCORES - This macros defines the maximum logical cores. Default value assigned is : 16. */
#define MAX_RX_QUEUES_PER_LCORE 8 /**< MAX_RX_QUEUES_PER_LCORE - This macros defines the maximum Rx queues one single logical core can manage. Default value assigned is : 8. */
#define MAX_TX_PKT_BURST 32 /**< MAX_TX_PKT_BURST - This macros defines the maximum Tx burst size so that the application doesn't have to send each packet individually. Default value assigned is : 32. */
#define MASTER_LCORE 0 /**< MASTER_LCORE - This macros defines the maximum logical cores. Default value assigned is : 0. */

struct lcore_port_queue {
	uint8_t port_id;  /**< port_id - This variable contains the information of the Port Id which the logical core is managing. */
	uint8_t queue_id; /**< queue_id - This variable contains the information of the Queue Id which the logical core is managing. */
};

struct lcore_conf {
	uint8_t socket_id;
	uint8_t num_queues;
	uint8_t core_rx;
	uint8_t core_tx;
	uint8_t core_working;
	uint8_t num_port;
	uint8_t managed_port[MAX_RX_QUEUES_PER_LCORE];
	struct lcore_port_queue mngd_queues[MAX_RX_QUEUES_PER_LCORE];
	struct mbuf_table tx_mbufs[MAX_ETH_PORTS];
} __rte_cache_aligned;

struct lcore_conf lcore_conf[MAX_LCORES]; /**< lcore_conf - This array of structure contains all the information of each individual logical core .  */

int lcore_conf_init(void); /**< lcore_conf_init - This function initialize all the lcore respective configurations.  */
int lcore_pipeline_init(void); /**< lcore_pipeline_init - This function initialize all the lcore respective configurations related to the pipeline model  */


#endif
