/**@file /* The timers created will be periodic.
 * They will be mainly used for printing and exporting stats, updating ARP table
 * This file will include following funcs
 * create timer with a periodic interval
 * update the timer status
 *
 */

#ifndef _PERIODIC_TIMERS_H_
#define _PERIODIC_TIMERS_H_

#include <timerfuncs.h>
#include <stats.h>
#include <rte_lcore.h>

uint64_t prev_periodic_freq;		//--! Alternate initialization is to create periodic_init func
uint64_t curr_periodic_freq;

/** Calls another function that does all the work.
 *
 * @bug Not Used
 */
inline void tw_init_periodic_timers(void);

/**Creates a periodic timer
 * @param timer_name :name of timer
 *
 * @param time_period :Time period of timer
 *
 * @param timer_lcore_id :ID of core on which it will run
 *
 * @param timer_cb :Callback assigned to the timer
 *
 * @param cb_func_args :Callback function's arguments
 *
 * @return 0(TRUE)
 */
int tw_create_periodic_timer(struct rte_timer * timer_name, uint64_t time_period,
        uint8_t timer_lcore_id, rte_timer_cb_t timer_cb, void * cb_func_args);

/** Used to update the timer to a new limit value
 *
 * @param time_limit : New time limit
 *
 * @return 0(TRUE)
 */
int tw_update_periodic_timers(uint64_t time_limit);

/** Calls the stats timercallback
 *
 * @return 0(TRUE)
 */
int tw_stats_callback_timer_init(void);

#endif
