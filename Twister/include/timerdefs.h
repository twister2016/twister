#ifndef _TIMEDEFS_H_
#define _TIMEDEFS_H_
#include <rte_timer.h>
#include <rte_cycles.h>

#define USE_PERIODIC_TIMERS 1

#define MILLION 1000000UL
#define BURST_TX_DRAIN_US 200

struct timestamp_option
{
	uint8_t type;
	uint8_t length;
	uint32_t timestamp;
	uint32_t echo_timestamp;
} __attribute__ ((__packed__));

uint64_t one_sec = 0, one_msec = 0, one_usec = 0, one_nsec = 0;
uint64_t prev_tsc = 0, cur_tsc = 0, timer_tsc = 0, diff_tsc = 0;

inline void sleep_msec(unsigned msec);
inline void sleep_usec(unsigned sec);
inline void sleep_nsec(unsigned nsec);

void init_timer_vals(void);

#endif