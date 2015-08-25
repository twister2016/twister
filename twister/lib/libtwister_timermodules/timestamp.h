#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_
/**
 * @file
 *
 * Timer Stamp Helpers in Twister
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <rte_memory.h>
#include <rte_memzone.h>
#include <rte_launch.h>
#include <rte_tailq.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <rte_mbuf.h>
#include <rte_byteorder.h>
#include <rte_cycles.h>
#include <rte_version.h>
#include <timerdefs.h>


#define TIMESTAMP_ENABLE 1  //TODO take from user json files.

static uint32_t last_received_timestamp = 0 ;

struct timestamp_option
{
	uint8_t type;
	uint8_t length;
	uint32_t timestamp;        
	uint32_t echo_timestamp;
} __attribute__ ((__packed__));


enum {
	TIMESTAMP_TYPE 	= 0x8,
	TIMESTAMP_LENGTH = 0xA
};

inline void add_timestamp (struct timestamp_option *timestamp)
{
	timestamp->type 		= TIMESTAMP_TYPE;
	timestamp->length 		= TIMESTAMP_LENGTH;
	timestamp->timestamp 		= rte_rdtsc()/one_usec;
	timestamp->echo_timestamp	= last_received_timestamp;
}

void parse_timestamp (struct timestamp_option *timestamp) {
	if(timestamp->type  == TIMESTAMP_TYPE) {
		last_received_timestamp = timestamp->timestamp;
		uint32_t curr_time = rte_rdtsc()/one_usec;		
		calc_average_rtt(abs(curr_time - timestamp->echo_timestamp));
	}
}

#endif

