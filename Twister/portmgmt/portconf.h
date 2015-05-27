#ifndef _PORTCONF_H_
#define _PORTCONF_H_

#include <rte_ethdev.h>
#include <memmgmt/mempoolconf.h>

#define MAX_ETH_PORTS 16
#define RX_QUEUES_PER_PORT 1
#define TX_QUEUES_PER_PORT 1	//--! SRIOV NICs only support 1 rx and 1 tx queue per port

#define ACCEPT_BRDCAST	1		//If the ports should accept the broadcast packets or not

#define RX_RING_DESC_DEFAULT 128
#define TX_RING_DESC_DEFAULT 512
static uint16_t nb_rxd = RX_RING_DESC_DEFAULT;
static uint16_t nb_txd = TX_RING_DESC_DEFAULT;

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
	struct mbuf_table tx_pkt_array[MAX_TX_QUEUES_PER_PORT];
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


int eth_port_init() {
	uint8_t port_id, counter;
	int ret, socket_id;
	total_eth_ports = rte_eth_dev_count();
	struct rte_eth_dev_info dev_info;
	if (total_eth_ports == 0)
		rte_exit(EXIT_FAILURE, "No Ethernet ports\n");
	if (total_eth_ports > MAX_ETH_PORTS)
		total_eth_ports = MAX_ETH_PORTS;
	available_eth_ports = total_eth_ports;

	for (port_id = 0; port_id < total_eth_ports; port_id++) {
		/* skip ports that are not enabled */
		if ((eth_port_mask & (1 << port_id)) == 0) {
			available_eth_ports--;
			continue;
		}
		ret = rte_eth_dev_configure(port_id, port_info[port_id].num_rx_queues, port_info[port_id].num_tx_queues, &port_conf);
		if (ret < 0)
			rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n",
				  ret, (unsigned) port_id);
		rte_eth_macaddr_get(port_id, port_info[port_id].eth_mac);
		socket_id = rte_eth_dev_socket_id(port_id);
		port_info[port_id].socket_id = socket_id;
		rte_eth_dev_info_get(portid, &dev_info);			//--!TODO use dev_info in port_info struct

		port_info[port_id].num_rx_queues = RX_QUEUES_PER_PORT;
		port_info[port_id].num_tx_queues = TX_QUEUES_PER_PORT;
		for(counter=0;counter<port_info[port_id].num_rx_queues;counter++) {
			ret = rte_eth_rx_queue_setup(port_id, 0, nb_rxd, socket_id, NULL, rx_mempool[socket_id]);
			if (ret < 0)
				rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret, (unsigned) port_id);
		}
		for(counter=0;counter<port_info[port_id].num_tx_queues;counter++) {
			ret = rte_eth_rx_queue_setup(port_id, 0, nb_rxd, socket_id, NULL, tx_mempool[socket_id]);
			if (ret < 0)
				rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret, (unsigned) port_id);
		}
	}
	return 0;
}

#endif
