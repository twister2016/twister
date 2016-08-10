/**@file Contains structs that have info about ports, buffers for each port.
 *
 */

#ifndef _PORTCONF_H_
#define _PORTCONF_H_

#include <rte_config.h>
#include <rte_ethdev.h>
#include "mempoolconf.h"
#include <tw_common.h>

#define MAX_ETH_PORTS 8
#define RX_QUEUES_PER_PORT 1
#define TX_QUEUES_PER_PORT 1	//--! SRIOV NICs only support 1 rx and 1 tx queue per port

#define ACCEPT_BRDCAST	1		//If the ports should accept the broadcast packets or not

#define DEFAULT_RX_RING_DESC 128
#define DEFAULT_TX_RING_DESC 512

uint16_t nb_rxd;
uint16_t nb_txd;

uint8_t total_eth_ports;
uint8_t available_eth_ports;
uint16_t app_port_mask;

enum
{
    REPLY_ARP = 0x00000001, REPLY_ICMPv4 = 0x00000002,	//--!TODO add support for icmpv4 and v6
    ENABLE_VLAN = 0x00000004,
    ENABLE_PROMSC = 0x00000008,
    MGMT_PORT = 0x00000010
}__attribute__((__packed__));

/** Table of every buffer, a list of all memory buffers.
 *
 */
struct mbuf_table
{
        int len;
        struct rte_mbuf *m_table[MAX_TX_PKT_BURST];
        uint8_t portid;
}__attribute__((__packed__));

/** Contains info about port like IPs, name etc.
 *
 */
struct port_info
{
        struct ether_addr * eth_mac;
        char port_name[10];
        uint8_t port_id_external;
        uint32_t start_ip_addr;	//One port can have IPs from same subnet, hence a single gateway and subnet mask
        uint8_t num_ip_addrs;
        uint32_t gateway_ip;
        uint32_t subnet_mask;
        uint16_t vlan_tag;
        uint64_t flags;
        uint8_t socket_id;
        uint8_t num_rx_queues;
        uint8_t num_tx_queues;
        struct mbuf_table tx_pkt_array[TX_QUEUES_PER_PORT];
}__attribute__((__packed__));

struct port_info port_info[MAX_ETH_PORTS];

static const struct rte_eth_conf port_conf =
{
    .rxmode =
    {
        .split_hdr_size = 0,
        .header_split = 0, /**< Header Split disabled */
        .hw_ip_checksum = 0, /**< IP checksum offload disabled */
        .hw_vlan_filter = 0, /**< VLAN filtering disabled */
        .jumbo_frame = 0, /**< Jumbo Frame Support disabled */
        .hw_strip_crc = 0, /**< CRC stripped by hardware */
    },
    .txmode =
    {
        .mq_mode = ETH_MQ_TX_NONE,
    },
};

#endif
