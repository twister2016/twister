#ifndef _TX_H_
#define _TX_H_
/**
 * @file
 *
 * TX Module Helpers in Twister
 */
#include <stdint.h>
#include <rte_config.h>
#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_ethdev.h>
#include <lcoreconf.h>
#include <timerdefs.h>
#include <stats.h>


int add_pkt_to_tx_queue (struct rte_mbuf *, uint8_t); /**< add_pkt_to_tx_queue - This function when call add the packet to the specified queue. Arguments include: packet and queue number. */
int twister_timely_burst(void); /**< twister_timely_burst - This function when call burst the packet in the Tx queue if a specified time has been passed. The time set in the Burst Time out Macro. */


#endif
