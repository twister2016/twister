#ifndef _PORTCONF_H_
#define _PORTCONF_H_

#include <rte_ethdev.h>
#include <mempoolconf.h>

#define MAX_ETH_PORTS 16
#define RX_QUEUES_PER_PORT 1
#define TX_QUEUES_PER_PORT 1	//--! SRIOV NICs only support 1 rx and 1 tx queue per port

#define ACCEPT_BRDCAST	1		//If the ports should accept the broadcast packets or not

#define DEFAULT_RX_RING_DESC 128
#define DEFAULT_TX_RING_DESC 512
uint16_t nb_rxd = DEFAULT_RX_RING_DESC;
uint16_t nb_txd = DEFAULT_TX_RING_DESC;

uint8_t total_eth_ports = 0;
uint8_t available_eth_ports = 0;
uint16_t eth_port_mask = 0;

enum {
	REPLY_ARP		= 0x00000001,
	REPLY_ICMPv4	= 0x00000002	//--!TODO add support for icmpv4 and v6
} __attribute__((__packed__));

struct mbuf_table {
	unsigned len;
	struct rte_mbuf *m_table[MAX_PKT_BURST];
} __attribute__((__packed__));

struct port_info {
	union {
		uint32_t	ip_addr;
		uint8_t		ip[4];
	};
	uint8_t	flags;
	struct ether_addr * eth_mac;		//--!TODO initialize eth_mac in init func
	uint16_t vlan_tag;
	uint8_t socket_id;
	uint8_t num_rx_queues;			//--!TODO init the values of rx and tx queues
	uint8_t num_tx_queues;
	struct mbuf_table tx_pkt_array[TX_QUEUES_PER_PORT];
	//struct port_stats;	//--! add stats info
} __attribute__((__packed__));

struct port_info port_info[MAX_ETH_PORTS];

static const struct rte_eth_conf port_conf = {
	.rxmode = {
		.split_hdr_size = 0,
		.header_split   = 0, /**< Header Split disabled */
		.hw_ip_checksum = 0, /**< IP checksum offload disabled */
		.hw_vlan_filter = 0, /**< VLAN filtering disabled */
		.jumbo_frame    = 0, /**< Jumbo Frame Support disabled */
		.hw_strip_crc   = 0, /**< CRC stripped by hardware */
	},
	.txmode = {
		.mq_mode = ETH_MQ_TX_NONE,
	},
};


int eth_port_init(void);

#endif
