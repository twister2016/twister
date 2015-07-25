#ifndef _TIMERFUNCS_H_
#define _TIMERFUNCS_H_

#include <timerdefs.h>

inline uint64_t get_time_diff(uint64_t, uint64_t, uint64_t);
inline uint8_t is_timer_expired(uint64_t, uint64_t, uint64_t, uint64_t);
inline uint64_t get_current_timer_cycles(void);
inline void sleep_msec(unsigned);
inline void sleep_usec(unsigned);
inline void sleep_nsec(unsigned);

#endif
