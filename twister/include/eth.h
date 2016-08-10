/** @file Ethernet layer functionality
 *
 */
#ifndef _ETH_H_
#define _ETH_H_

#include <mempoolconf.h>
#include <inttypes.h>
#include <rte_ether.h>
#include <mempoolconf.h>
#include <event_loop.h>
#include <tw_api.h>
#include <tw_common.h>

uint8_t eth_hdr_size;

/** Ethernet packet constructor
 *
 * @param m :Pointer to packet
 *
 * @param port_id: ID of port
 *
 * @param eth_type: Type of Ethernet packet
 *
 * @param dst_ip: Destination IP
 *
 * @return 0(TRUE)
 */
int tw_eth_pkt_ctor(struct rte_mbuf* m, uint8_t port_id, uint16_t eth_type, uint32_t dst_ip);

/** Parses any ethernet packet received
 *
 * @param pkt :Pointer to received Ethernet packet
 *
 * @param port_id: ID of port
 *
 * @return 0(TRUE)
 */
int tw_eth_pkt_parser(struct rte_mbuf * pkt, uint8_t port_id);

#endif
