#ifndef _TIMERFUNCS_H_
#define _TIMERFUNCS_H_

#include <timerdefs.h>

inline uint64_t tw_get_time_diff(uint64_t, uint64_t, uint64_t);
inline uint8_t tw_is_timer_expired(uint64_t, uint64_t, uint64_t, uint64_t);
inline uint64_t tw_get_current_timer_cycles(void);
inline uint8_t tw_if_timeout_millisec(uint64_t, uint64_t, uint64_t);

#endif
