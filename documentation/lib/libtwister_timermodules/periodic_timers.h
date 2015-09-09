#ifndef _PERIODIC_TIMERS_H_
#define _PERIODIC_TIMERS_H_
/**
 * @file
 *
 * Periodic Timers Helpers in Twister
 */
#include <timerfuncs.h>
#include <stats.h>
#include <rte_lcore.h>

extern uint64_t prev_periodic_freq;					//--! Alternate initialization is to create periodic_init func
extern uint64_t curr_periodic_freq;

inline void init_periodic_timers(void); /**< init_periodic_timers - This function initialize all the periodic timers. */
int create_periodic_timer(struct rte_timer *, uint64_t, uint8_t, rte_timer_cb_t, void *); /**< create_periodic_timer - This function creates a periodic timer with specified arguments. */
int update_periodic_timers(uint64_t); /**< update_periodic_timers - This function updates the periodic timer values. */
int stats_callback_timer_init(void); 
#endif
