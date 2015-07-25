#ifndef _TIMERFUNCS_H_
#define _TIMERFUNCS_H_

#include <timerdefs.h>

inline uint64_t get_time_diff(uint64_t, uint64_t, uint64_t); /**< get_time_diff - This function calculate the time difference between two time. */
inline uint8_t is_timer_expired(uint64_t, uint64_t, uint64_t, uint64_t); /**< is_timer_expired - This function checks whether the timer has expired or not. */
inline uint64_t get_current_timer_cycles(void); /**< get_current_timer_cycles - This function gets the current timer cycle from the DPDK built-in function. */
inline void sleep_msec(unsigned); /**< sleep_msec - This function sleeps the application for the respective milliseconds as specified. */
inline void sleep_usec(unsigned); /**< sleep_usec - This function sleeps the application for the respective microseconds as specified. */
inline void sleep_nsec(unsigned); /**< sleep_nsec - This function sleeps the application for the respective nanoseconds as specified. */

#endif
