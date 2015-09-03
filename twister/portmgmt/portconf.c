#ifndef _PORTCONF_C_
#define _PORTCONF_C_

#include <portconf.h>


uint16_t nb_rxd = DEFAULT_RX_RING_DESC;
uint16_t nb_txd = DEFAULT_TX_RING_DESC;

uint8_t total_eth_ports = 0;
uint8_t available_eth_ports = 0;
uint16_t app_port_mask = 0;

int eth_name_to_id(char* portName){
	int i;
	int prt_id=-1;
	for(i=0;i<MAX_ETH_PORTS;i++){
	    //printf("portinfo[%d].port_name=%s\n", i,port_info[i].port_name );
	    if ( port_info[i].port_name!=NULL ) {
	        if(strcmp(port_info[i].port_name, portName) == 0)
	        {
	        prt_id=i;
	        //printf ("strcmp is zero now =%d with port_info(%s) with argument=%s\n",prt_id, port_info[i].port_name, portName);
	        }
			    
	    }
	    else{
	    //printf("NULL VALUE FROM JSON.eth_name_to_id....................\n");
	    //printf("%s\n", portName);
	    //printf("%d\n", i);
	    
	    }
		
	}
	//printf ("ending successfully eth_name_to_id with porti=%d\n",prt_id);
	return prt_id;
}

int get_port_by_ip(uint32_t ip_addr)
{
	if(ip_addr == 0) {
		//printf("IP addr is 0\n");
		return -1;
	}
    	uint8_t i=0;
    	for (i=0; i<MAX_ETH_PORTS; i++)
	{
	    	if(port_info[i].start_ip_addr == ip_addr)
		{
	        	return i;
		}
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

	//*****************PATCH*************************
	
	//uint8_t port_info_counter=0;
	//char eth_string[10];
	
	
	for (port_id = 0; port_id < total_eth_ports; port_id++) {
		/* skip ports that are not enabled */
		if ((app_port_mask & (1 << port_id)) == 0) {
			available_eth_ports--;
			continue;
		}
		//sprintf(eth_string, "tw%d", port_info_counter);
		//port_info[port_id].port_id_external = port_info_counter;
		//port_info[port_id].port_name= eth_string;
		//port_info_counter++;
		//TODO Setting RX TX QUEUES #1
		//port_info[port_id].num_rx_queues =1;
		//port_info[port_id].num_tx_queues = 1;
		
		//*****************PATCH*************************
		
		
		printf("port_id %d, num_rx_queues %d, num_tx_queues %d\n", port_id, port_info[port_id].num_rx_queues, port_info[port_id].num_tx_queues);
		ret = rte_eth_dev_configure(port_id, port_info[port_id].num_rx_queues, port_info[port_id].num_tx_queues, &port_conf);
		//ret = rte_eth_dev_configure(port_id, 1, 1, &port_conf);
		if (ret < 0)
			rte_exit(EXIT_FAILURE, "Cannot configure device: err=%d, port=%u\n",
				  ret, (unsigned) port_id);
		rte_eth_promiscuous_enable(port_id);
		port_info[port_id].eth_mac = rte_malloc("struct ether_addr", sizeof(struct ether_addr), RTE_CACHE_LINE_SIZE);
		rte_eth_macaddr_get(port_id, port_info[port_id].eth_mac);

		printf("Port %u, MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n\n",
				(unsigned) port_id,
				port_info[port_id].eth_mac->addr_bytes[0],
				port_info[port_id].eth_mac->addr_bytes[1],
				port_info[port_id].eth_mac->addr_bytes[2],
				port_info[port_id].eth_mac->addr_bytes[3],
				port_info[port_id].eth_mac->addr_bytes[4],
				port_info[port_id].eth_mac->addr_bytes[5]);

		socket_id = rte_eth_dev_socket_id(port_id);
		if(socket_id == -1)
			socket_id = 0;
		port_info[port_id].socket_id = socket_id;
		rte_eth_dev_info_get(port_id, &dev_info);			//--!TODO use dev_info in port_info struct
		printf("%d port_id, %d socket id, %d num rx q, %d num tx q\n", port_id, port_info[port_id].socket_id, port_info[port_id].num_rx_queues, port_info[port_id].num_tx_queues);
		for(counter=0;counter<port_info[port_id].num_rx_queues;counter++) {
			ret = rte_eth_rx_queue_setup(port_id, counter, nb_rxd,rte_eth_dev_socket_id(port_id), NULL, rx_mempool[socket_id]);
			if (ret < 0)
				rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret, (unsigned) port_id);
		}
		for(counter=0;counter<port_info[port_id].num_tx_queues;counter++) {
			ret = rte_eth_tx_queue_setup(port_id, counter, nb_txd, rte_eth_dev_socket_id(port_id), NULL);
			if (ret < 0)
				rte_exit(EXIT_FAILURE, "rte_eth_rx_queue_setup:err=%d, port=%u\n", ret, (unsigned) port_id);
		}
		
		ret = rte_eth_dev_start(port_id);
		if(ret < 0)
			rte_panic("Cannot start port %u (%d)\n", port_id, ret);
	}
	return 0;
}

void check_all_ports_link(void) {
	uint32_t all_ports_up, port_id;

	all_ports_up = 1;

	for (port_id = 0; port_id < available_eth_ports; port_id++) {
		struct rte_eth_link link;
		memset(&link, 0, sizeof(link));
		rte_eth_link_get_nowait(port_id, &link);
		RTE_LOG(INFO, USER1, "Port %u (%u Gbps) %s\n",
			port_id,
			link.link_speed / 1000,
			link.link_status ? "UP" : "DOWN");

		if (link.link_status == 0)
			all_ports_up = 0;
	}

	if (all_ports_up == 0)
		rte_panic("Some NIC ports are DOWN\n");
	return;
}
/*
void print_port_info(void) {
	int i = 0;
	for(i = 0; i < MAX_ETH_PORTS; i++) {
		printf("port name %s, extern id %d, start ip %d, num ip addrs %d, socket_id %d, num_rx_queues %d num_tx_queues %dn", port_info[i].port_name, port_info[i].port_id_external, );
	}	
	return;
}
*/

#endif
