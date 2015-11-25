#ifndef _ENGINE_C_
#define _ENGINE_C_

#include <tw_api.h>

void tw_launch_engine(void * func, void * arg, char* engine_name) {
	int engine_id = tw_engine_name_to_lcore_id(engine_name);
        
/*	if(launch_type == USE_ALL_ENGINES)
		rte_eal_mp_remote_launch(func, arg, CALL_MASTER);
	else if(launch_type == RESERVE_FIRST_ENGINE)
		rte_eal_mp_remote_launch(func, arg, SKIP_MASTER);
	else if(launch_type == ENGINE0) {
		int master = rte_get_master_lcore();
		int (*temp_func) (void *) = func;
		lcore_config[master].ret = temp_func(arg);
		lcore_config[master].state = FINISHED;
	}
	else
*/		rte_eal_remote_launch(func, arg, engine_id);
		
	return;
}

unsigned tw_current_engine(void) {
	return rte_lcore_id();
}

#endif
