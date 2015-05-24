#ifndef _TIMEDEFS_H_
#define _TIMEDEFS_H_
#include <rte_timer.h>
#include <rte_cycles.h>

#define MILLION 1000000UL
#define BURST_TX_DRAIN_US 200

/*************************************************/
void init_timer_vals(void);
/*************************************************/

struct timestamp_option
{
	uint8_t type;
	uint8_t length;
	uint32_t timestamp;
	uint32_t echo_timestamp;
} __attribute__ ((__packed__));

uint64_t one_sec = 0, one_msec = 0, one_usec = 0, one_nsec = 0;
uint64_t prev_tsc = 0, cur_tsc = 0, timer_tsc = 0, diff_tsc = 0;

static inline void sleep_msec(unsigned msec);
static inline void sleep_usec(unsigned sec);
static inline void sleep_nsec(unsigned nsec);

void init_timer_vals(void) {
	one_sec = rte_get_tsc_hz();
	one_msec = (one_sec/1000UL);
	one_usec = (one_msec/1000UL);
	one_nsec = (one_usec/1000UL);
}

/**
 * This function provides a sleep in msec
 */
static inline void sleep_msec(unsigned msec) {	//--! should it be in the timer_functions file?
	uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles()/one_msec;
	while(curr_sleep - prev_sleep <= msec) 
		curr_sleep = rte_get_tsc_cycles()/one_msec;
	return;
}
/**
 * This function provides a sleep in usec
 */
static inline void sleep_usec(unsigned usec) {	//--! should it be in the timer_functions file?
	uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles()/one_usec;
	while(curr_sleep - prev_sleep <= usec)
		curr_sleep = rte_get_tsc_cycles()/one_usec;
	return;
}
/**
 * This function provides a sleep in nsec
 */
static inline void sleep_nsec(unsigned nsec) {	//--! should it be in the timer_functions file?
	uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles()/one_nsec;
	while(curr_sleep - prev_sleep <= nsec)
		curr_sleep = rte_get_tsc_cycles()/one_nsec;
	return;
}

#endif
