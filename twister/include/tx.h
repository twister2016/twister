/** @file Contains all Tx functionality
 *
 */

#ifndef _TX_H_
#define _TX_H_

#include <stdint.h>
#include <rte_config.h>
#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_ethdev.h>
#include <lcoreconf.h>
#include <timerdefs.h>
#include <stats.h>

/** Adds the packet to the queue of packets to send and
 * if the limit(32) is reached then it sends a burst
 *
 * @param m :Pointer to the packet to send
 *
 * @param port :Port from which to send the packet
 *
 * @return 0(TRUE)
 */
int tw_add_pkt_to_tx_queue(struct rte_mbuf * m, uint8_t port);

/** Used to send bursts if certain time has passed(100ms)
 *
 * @return 0(TRUE)
 */
int tw_timely_burst(void);

#endif
