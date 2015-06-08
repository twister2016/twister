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
			lcore_conf[lcore_id].num_queues = 0;
			//map_queue_to_core();						//--!TODO implement assign_queue_to_core
		}
	}
	return 0;
}

/*
int map_queue_to_core(uint8_t core_id, uint8_t port_id, uint8_t queue_id) {
	if(lcore_conf[core_id].socket_id == port_info[port_id].socket_id) {
		lcore_conf[core_id].managed_port[(lcore_conf[core_id].num_queues) - 1] = port_id;
		lcore_conf[core_id].num_queues += 1;
		return 0;
	}
	else
		printf("The core %d and port %d have different NUMA nodes\n", core_id, port_id);
	return -1;
}
*/

#endif
