/** @file Functions for sleep timers
 *
 */

#ifndef _TIMEDEFS_H_
#define _TIMEDEFS_H_
#include <rte_timer.h>
#include <rte_cycles.h>

#define USE_PERIODIC_TIMERS 1

#define MILLION 1000000UL
#define BURST_TX_DRAIN_US 200

uint64_t one_sec, one_msec, one_usec, one_nsec;
uint64_t prev_tsc, cur_tsc, timer_tsc, diff_tsc;

/** Sleep for specified milliseconds
 *
 * @param msec :Time in milliseconds
 */
inline void tw_sleep_msec(unsigned msec);

/** Sleep for specified microseconds
 *
 * @param usec :Time in microseconds
 */
inline void tw_sleep_usec(unsigned usec);

/** Sleep for specified nanoseconds
 *
 * @param nsec :Time in nanoseconds
 */
inline void tw_sleep_nsec(unsigned nsec);

/** Initializes timer constants and call rte function
 *
 */
void tw_init_timer_vals(void);

#endif
