#ifndef _PERIODIC_TIMERS_H_
#define _PERIODIC_TIMERS_H_

#include <timerfuncs.h>

uint64_t prev_periodic_freq = 0;					//--! Alternate initialization is to create periodic_init func
uint64_t curr_periodic_freq = 0;

inline void init_periodic_timers(void);
int create_periodic_timer(struct rte_timer *, uint32_t, uint8_t, rte_timer_cb_t, void *);
int update_periodic_timers(uint64_t);

#endif
