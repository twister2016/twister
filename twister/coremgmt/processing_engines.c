#ifndef _ENGINE_C_
#define _ENGINE_C_

#include <processing_engines.h>

void tw_launch_processing_engines(void * func, void * arg, int launch_type) {
	
	if(launch_type == USE_ALL_ENGINES)
		rte_eal_mp_remote_launch(func, arg, CALL_MASTER);
	else if(launch_type == RESERVE_FIRST_ENGINE)
		rte_eal_mp_remote_launch(func, arg, SKIP_MASTER);
	
	return;
}

#endif
