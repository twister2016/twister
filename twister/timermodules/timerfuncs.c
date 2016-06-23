#include <timerfuncs.h>

inline uint64_t tw_get_time_diff(uint64_t curr_timer_freq, uint64_t prev_timer_freq,
                                 uint64_t timer_resolution)
{
    return abs((curr_timer_freq - prev_timer_freq) / timer_resolution);
}

inline uint8_t tw_is_timer_expired(uint64_t curr_timer_freq, uint64_t prev_timer_freq,
                                   uint64_t timer_resolution, uint64_t time_limit)
{
    uint64_t time_diff = tw_get_time_diff(curr_timer_freq, prev_timer_freq, timer_resolution);
    if(time_diff < time_limit)
        return 0;
    else
        return 1;
}

inline void tw_get_timer_hz(uint64_t *val)
{
    *val =  rte_get_tsc_hz();
}

inline uint64_t tw_get_current_timer_cycles(void)
{
    return rte_get_timer_cycles();
}

inline uint8_t tw_if_timeout_millisec(uint64_t curr_timer_cycles, uint64_t prev_timer_cycles,
                                      uint64_t num_millisecs)
{
    return tw_is_timer_expired(curr_timer_cycles, prev_timer_cycles, one_msec, num_millisecs);
}

/**
 * This function provides a sleep in msec
 */
inline void tw_sleep_msec(unsigned msec)
{
    uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles() / one_msec;
    while (curr_sleep - prev_sleep <= msec)
        curr_sleep = rte_get_tsc_cycles() / one_msec;
    return;
}
/**
 * This function provides a sleep in usec
 */
inline void tw_sleep_usec(unsigned usec)
{
    uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles() / one_usec;
    while (curr_sleep - prev_sleep <= usec)
        curr_sleep = rte_get_tsc_cycles() / one_usec;
    return;
}
/**
 * This function provides a sleep in nsec
 */
inline void tw_sleep_nsec(unsigned nsec)
{
    uint64_t curr_sleep = 0, prev_sleep = rte_get_tsc_cycles() / one_nsec;
    while (curr_sleep - prev_sleep <= nsec)
        curr_sleep = rte_get_tsc_cycles() / one_nsec;
    return;
}
