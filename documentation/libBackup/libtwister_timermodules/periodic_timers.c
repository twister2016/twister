#ifndef _PERIODIC_TIMERS_C_
#define _PERIODIC_TIMERS_C_

/* The timers created will be periodic. They will be mainly used for printing and exporting stats, updating ARP table
 * This file will include following funcs
 * create timer with a periodic interval
 * update the timer status
 */
#include <stats.h>
#include <periodic_timers.h>
uint64_t curr_periodic_freq = 0;
uint64_t prev_periodic_freq = 0;	


inline void init_periodic_timers(void) {
	prev_periodic_freq = get_current_timer_cycles();
	return;
}

int create_periodic_timer(struct rte_timer * timer_name, uint64_t time_period, \
				uint8_t timer_lcore_id, rte_timer_cb_t timer_cb, void * cb_func_args) {
	rte_timer_init(timer_name);
	rte_timer_reset(timer_name, time_period, PERIODICAL, timer_lcore_id, timer_cb, cb_func_args);	//This will also start the timer
	return 0;
}

int update_periodic_timers(uint64_t time_limit) {							//Time limit will be in millisecs
	curr_periodic_freq = get_current_timer_cycles();
	if (is_timer_expired(curr_periodic_freq, prev_periodic_freq, one_msec, time_limit)) {
		rte_timer_manage();
		prev_periodic_freq = curr_periodic_freq;
	}
	return 0;
}


int stats_callback_timer_init(void) {

    struct rte_timer stat_timer;
    int tem= create_periodic_timer(&stat_timer, stats_time_period, rte_lcore_id(), (rte_timer_cb_t) print_global_stats, NULL);
    if (tem!=0)
        printf ("error occured at stats timer init, returned=%d\n", tem);
        
    return 0;
}

#endif
