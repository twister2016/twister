/** @file Contains all the functionality related to core configuration
 *
 */

#ifndef _LCORECONF_H_
#define _LCORECONF_H_

#include <rte_per_lcore.h>
#include <portconf.h>
#include <tw_common.h>

#define MAX_LCORES 16
#define MAX_RX_QUEUES_PER_LCORE 8
#define MAX_TX_PKT_BURST 32
#define MASTER_LCORE 0

/** Contains all the info about a core or engine
 *
 */
struct lcore_conf
{
        char core_name[10];
        uint8_t core_id_external;
        uint8_t socket_id;
        uint8_t core_rx;
        uint8_t core_tx;
        uint8_t core_working;
        uint8_t num_port;
        uint8_t managed_port[MAX_ETH_PORTS];
        struct mbuf_table tx_mbufs[MAX_ETH_PORTS];
}__rte_cache_aligned;

struct lcore_conf lcore_conf[MAX_LCORES];

/** Creates core masks. Hard coded.
 * @param
 * @param
 * @return
 */
int tw_lcore_conf_init(void);

/** Takes the engine name and returns its lcore ID
 *
 * @param engineName :Name of engine in string form
 *
 * @return ID fo engine
 */
int tw_engine_name_to_lcore_id(char* engineName);

#endif
