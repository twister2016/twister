#ifndef _COMMON_H_
#define _COMMON_H_

#include "timerdefs.h"

static inline uint64_t timer_diff(uint64_t timer_resolution) {
	cur_tsc = rte_get_tsc_cycles()/timer_resolution;
	return abs(cur_tsc - prev_tsc);
}

static inline uint8_t timer_sync( uint64_t time_limit, uint64_t timer_resolution ) {	//--! register callback functions to run...will need a generic model for function pointers
	if(timer_diff(timer_resolution) < time_limit)
		return 0;
	else
		return 1;
}

#endif