#ifndef _PERIODIC_TIMERS_H_
#define _PERIODIC_TIMERS_H_

#include <timerfuncs.h>
#include <stats.h>
#include <rte_lcore.h>

extern uint64_t prev_periodic_freq;					//--! Alternate initialization is to create periodic_init func
extern uint64_t curr_periodic_freq;

inline void tw_init_periodic_timers(void);
int tw_create_periodic_timer(struct rte_timer *, uint64_t, uint8_t, rte_timer_cb_t, void *);
int tw_update_periodic_timers(uint64_t);
int tw_stats_callback_timer_init(void);
#endif
