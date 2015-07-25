#ifndef _TIMEDEFS_H_
#define _TIMEDEFS_H_
#include <rte_timer.h>
#include <rte_cycles.h>
/**
 * @file
 *
 * Timer Definitions Helpers in Twister
 */
#define USE_PERIODIC_TIMERS 1

#define MILLION 1000000UL
#define BURST_TX_DRAIN_US 200 /**< BURST_TX_DRAIN_US - This macros defines the time in microseconds after which the packets residing in the queue will be send. */

extern uint64_t one_sec, one_msec , one_usec , one_nsec ;
extern uint64_t prev_tsc, cur_tsc , timer_tsc , diff_tsc ;

inline void sleep_msec(unsigned msec); /**< sleep_msec - This function sleeps the application for the respective milliseconds as specified. */
inline void sleep_usec(unsigned sec); /**< sleep_usec - This function sleeps the application for the respective microseconds as specified. */
inline void sleep_nsec(unsigned nsec); /**< sleep_nsec - This function sleeps the application for the respective nanoseconds as specified. */

void init_timer_vals(void); /**< init_timer_vals - This function is called at the start of the application initialization and it initialize all the timers variables. */

#endif
