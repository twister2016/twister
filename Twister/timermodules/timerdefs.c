#ifndef _TIMEDEFS_C_
#define _TIMEDEFS_C_

#include <timerdefs.h>

void init_timer_vals(void) {
	one_sec = rte_get_timer_hz();
	one_msec = (one_sec/1000UL);
	one_usec = (one_msec/1000UL);
	one_nsec = (one_usec/1000UL);
#ifdef USE_PERIODIC_TIMERS
	rte_timer_subsystem_init();
#endif
}

#endif
