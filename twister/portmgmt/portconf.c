#ifndef _PORTCONF_C_
#define _PORTCONF_C_

#include <portconf.h>


uint16_t nb_rxd = DEFAULT_RX_RING_DESC;
uint16_t nb_txd = DEFAULT_TX_RING_DESC;

uint8_t total_eth_ports = 0;
uint8_t available_eth_ports = 0;
uint16_t eth_port_mask = 0;




int getportbyip(uint32_t ip_addr)
{
    uint8_t i=0;
    for (i=0; i<MAX_ETH_PORTS; i++)
	{
	    if (port_info[i].start_ip_addr == ip_addr)
	        return i;
	}
	
	return -1;
	
}
int eth_port_init(void) {
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
		rte_eth_dev_info_get(port_id, &dev_info);			//--!TODO use dev_info in port_info struct

		//port_info[port_id].num_rx_queues = RX_QUEUES_PER_PORT;	//--!The number of queues values are init before this func call by json init funcs
		//port_info[port_id].num_tx_queues = TX_QUEUES_PER_PORT;
		for(counter=0;counter<port_info[port_id].num_rx_queues;counter++) {
			ret = rte_eth_rx_queue_setup(port_id, 0, nb_rxd, socket_id, NULL, rx_mempool[socket_id]);
			if (ret < 0)
				rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret, (unsigned) port_id);
		}
		for(counter=0;counter<port_info[port_id].num_tx_queues;counter++) {
			ret = rte_eth_tx_queue_setup(port_id, 0, nb_txd, socket_id, NULL);
			if (ret < 0)
				rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret, (unsigned) port_id);
		}
	}
	return 0;
}

#endif
