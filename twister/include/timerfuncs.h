/** @file Contains all timer functions
 *
 */

#ifndef _TIMERFUNCS_H_
#define _TIMERFUNCS_H_

#include <timerdefs.h>

/** Retrieves the clock frequency
 *
 * @param val :Pointer to where the value will be stored
 */
inline void tw_get_timer_hz(uint64_t * val);

/** Finds the difference between two time values
 *
 * @param curr_time_freq :Current time(Maybe in cycles)
 *
 * @param prev_time_freq :Previous time(Maybe in cycles)
 *
 * @param timer_resolution :Timer resolution
 *
 * @return Difference in time
 */
inline uint64_t tw_get_time_diff(uint64_t curr_time_freq, uint64_t prev_time_freq,
		uint64_t timer_resolution);

/** Checks if the timer has expired by comparing the difference to the limit
 *
 * @param curr_time_freq :Current time(Maybe in cycles)
 *
 * @param prev_time_freq :Previous time(Maybe in cycles)
 *
 * @param timer_resolution :Timer resolution
 *
 * @param time_limit :Limit of timer i.e. value after which it is expired
 *
 * @return 0 if not expired and 1 if expired
 */
inline uint8_t tw_is_timer_expired(uint64_t curr_time_freq, uint64_t prev_time_freq,
		uint64_t timer_resolution, uint64_t time_limit);

/** Finds the current number of cycles
 *
 * @return Current cycles
 */
inline uint64_t tw_get_current_timer_cycles(void);

/** Checks if timer has expired with 1 msec as resolution
 *
 * @param curr_time_freq :Current time(Maybe in cycles)
 *
 * @param prev_time_freq :Previous time(Maybe in cycles)
 *
 * @param num_millisecs :Number of milliseconds.
 *
 * @return 0 if not expired and 1 if expired
 */
inline uint8_t tw_if_timeout_millisec(uint64_t curr_time_freq, uint64_t prev_time_freq,
		uint64_t num_millisecs);

#endif
