#ifndef _TIMEDEFS_H_
#define _TIMEDEFS_H_


#define BURST_TX_DRAIN_US 200
struct timestamp_option
{
	uint8_t type;
	uint8_t length;
	uint32_t timestamp;
	uint32_t echo_timestamp;
} __attribute__ ((__packed__));


uint64_t prev_tsc = 0, prev_arp = 0, prev_disp = 0, cur_tsc = 0, timer_tsc = 0;

static uint64_t one_msec		= 0;
static uint64_t one_sec			= 0;
static uint64_t one_usec 		= 0;
static uint64_t one_nsec 		= 0;

uint64_t diff_tsc abs(cur_tsc - prev_tsc)



#endif