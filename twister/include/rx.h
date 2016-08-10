/** @file All functionality related to reception of packet.
 *
 */

#ifndef _RX_H_
#define _RX_H_

#include <stdint.h>
#include <rte_config.h>
#include <rte_common.h>
#include <rte_cycles.h>
#include <rte_ethdev.h>
#include <lcoreconf.h>
#include <stats.h>

/** Gets any received packet from specified queue
 *
 * @param m :Table of all queues
 *
 * @return Number of received packets
 */
int tw_get_pkt_from_rx_queue(struct mbuf_table * m, uint8_t port, uint8_t queue_id);

/** All get packet for all queues
 *
 * @param m :Table of all queues
 *
 * @return Total packets received
 */
int tw_rx_for_each_queue(struct mbuf_table * m);

#endif
