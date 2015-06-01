#ifndef _COMMON_H_
#define _COMMON_H_

#include "timerdefs.h"

static inline uint64_t get_time_diff(uint64_t curr_timer_freq, uint64_t prev_timer_freq, uint64_t timer_resolution) {
	return abs((curr_timer_freq - prev_timer_freq)/timer_resolution);
}

static inline uint8_t is_timer_expired(uint64_t curr_timer_freq, uint64_t prev_timer_freq, uint64_t timer_resolution, uint64_t time_limit) {
	uint64_t time_diff = get_time_diff(curr_timer_freq, prev_timer_freq, timer_resolution);
	if(timer_diff < time_limit)
		return 0;
	else		
		return 1;
}

inline uint64_t get_current_timer_cycles() {
	return rte_get_timer_cycles();
}

/**
 * This function provides a sleep in msec
 */
static inline void sleep_msec(unsigned msec) {
	uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles()/one_msec;
	while(curr_sleep - prev_sleep <= msec) 
		curr_sleep = rte_get_tsc_cycles()/one_msec;
	return;
}
/**
 * This function provides a sleep in usec
 */
static inline void sleep_usec(unsigned usec) {
	uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles()/one_usec;
	while(curr_sleep - prev_sleep <= usec)
		curr_sleep = rte_get_tsc_cycles()/one_usec;
	return;
}
/**
 * This function provides a sleep in nsec
 */
static inline void sleep_nsec(unsigned nsec) {
	uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles()/one_nsec;
	while(curr_sleep - prev_sleep <= nsec)
		curr_sleep = rte_get_tsc_cycles()/one_nsec;
	return;
}

#endif
