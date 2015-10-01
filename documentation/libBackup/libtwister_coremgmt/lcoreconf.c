
#include <initfuncs.h>
#include <lcoreconf.h>

void assign_ports_to_cores(void);
int map_queue_to_core(uint8_t core_id, uint8_t port_id);

int map_queue_to_core(uint8_t core_id, uint8_t port_id) {
	if(lcore_conf[core_id].socket_id == port_info[port_id].socket_id) {
		lcore_conf[core_id].managed_port[lcore_conf[core_id].num_queues] = port_id;
		lcore_conf[core_id].num_port += 1;
		lcore_conf[core_id].num_queues += 1;
		printf("The core %d is managing port %d \n", core_id, port_id);
		return 0;
	}
	else
		printf("The core %d and port %d have different NUMA nodes\n", core_id, port_id);
	return -1;
}

int lcore_conf_init(void) {
	uint8_t lcore_id = 0;
	for(lcore_id=0;lcore_id<MAX_LCORES;lcore_id++) {
		if(rte_lcore_is_enabled(lcore_id)) {
			lcore_conf[lcore_id].socket_id = rte_lcore_to_socket_id(lcore_id);
			if(rte_lcore_to_socket_id(lcore_id))
				num_numa_sockets = 2;						//--!Considering there are 2 CPU sockets: Socket 0 and 1
			lcore_conf[lcore_id].num_queues = 0;
			lcore_conf[lcore_id].num_port = 0;
			map_queue_to_core(lcore_id,lcore_conf[lcore_id].mngd_queues[lcore_conf[lcore_id].num_queues].port_id);
			
								//--!TODO implement assign_queue_to_core
		}
	}
	return 0;
}

int lcore_pipeline_init(void){
	uint8_t lcore_id = 0;
	for(lcore_id=0;lcore_id<MAX_LCORES;lcore_id++) {
		if(rte_lcore_is_enabled(lcore_id)) {
			
			if(lcore_conf[lcore_id].core_working==1)
			{
				app.core_worker = lcore_id;
			}
			if(lcore_conf[lcore_id].core_rx==1)
			{
				app.core_rx = lcore_id;
			}
			if(lcore_conf[lcore_id].core_tx==1)
			{
				app.core_tx = lcore_id;
			}
								//--!TODO implement assign_queue_to_core
		}
	}
	return 0;
	
}

