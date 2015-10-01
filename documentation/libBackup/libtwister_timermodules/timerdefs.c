
#include <timerdefs.h>

uint64_t one_sec = 0, one_msec = 0, one_usec = 0, one_nsec = 0;
uint64_t prev_tsc = 0, cur_tsc = 0, timer_tsc = 0, diff_tsc = 0;

void init_timer_vals(void) {
	one_sec = rte_get_timer_hz();
	one_msec = (one_sec/1000UL);
	one_usec = (one_msec/1000UL);
	one_nsec = (one_usec/1000UL);
#ifdef USE_PERIODIC_TIMERS
	rte_timer_subsystem_init();
#endif
}


