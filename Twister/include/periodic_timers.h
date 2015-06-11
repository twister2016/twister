#ifndef _PERIODIC_TIMERS_H_
#define _PERIODIC_TIMERS_H_

#include <timerfuncs.h>

extern uint64_t prev_periodic_freq;					//--! Alternate initialization is to create periodic_init func
extern uint64_t curr_periodic_freq;

inline void init_periodic_timers(void);
int create_periodic_timer(struct rte_timer *, uint32_t, uint8_t, rte_timer_cb_t, void *);
int update_periodic_timers(uint64_t);

#endif
