#ifndef _LCORECONF_C_
#define _LCORECONF_C_

#include <lcoreconf.h>

int lcore_conf_init(void) {
	uint8_t lcore_id = 0;
	for(lcore_id=0;lcore_id<MAX_LCORES;lcore_id++) {
		if(rte_lcore_is_enabled(lcore_id)) {
			lcore_conf[lcore_id].socket_id = rte_lcore_to_socket_id(lcore_id);
			if(rte_lcore_to_socket_id(lcore_id))
				num_numa_sockets = 2;						//--!Considering there are 2 CPU sockets: Socket 0 and 1
			lcore_conf[lcore_id].num_ports = 0;
			port_to_lcore_mapping();						//--!TODO implement port to core mapping
		}
	}
	return 0;
}

int port_to_lcore_mapping(void) {
	return 0;
}

int assign_port_to_core(uint8_t core_id, uint8_t port_id) {
	if(lcore_conf[core_id].socket_id == port_info[port_id].socket_id) {
		lcore_conf[core_id].managed_port[(lcore_conf[core_id].num_ports) - 1] = port_id;
		lcore_conf[core_id].num_ports += 1;
		return 0;
	}
	else
		printf("The core %d and port %d have different NUMA nodes\n", core_id, port_id);
	return -1;
}

#endif