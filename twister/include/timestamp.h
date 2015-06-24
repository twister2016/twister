#ifndef _TIMESTAMP_H_
#define _TIMESTAMP_H_

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

#include "timerdefs.h"


#define TIMESTAMP_ENABLE 1  //TODO take from user json files.
#define TIMESTAMP_LENGTH 10 
#define TIMESTAMP_TYPE   8

static uint32_t last_received_timestamp = 0 ;

/*struct timestamp_option
{
	uint8_t type;
	uint8_t length;
	uint32_t timestamp;        //TODO figure out if timesamp_option should be here or in timedefs.h Currenlty in timerdefs.h
	uint32_t echo_timestamp;
} __attribute__ ((__packed__));
*/
static inline void addTcpOptions (struct rte_mbuf *pkt)
{
	if(TIMESTAMP_ENABLE)
	{
		if(pkt->pkt_len == 0)
			rte_pktmbuf_append(pkt, 12);
		else
			rte_pktmbuf_prepend(pkt, 12);
			
		struct timestamp_option *timestamp = rte_pktmbuf_mtod(pkt, struct timestamp_option *);
		timestamp->type 			= 8;//(uint8_t)TIMESTAMP_TYPE;
		timestamp->length 			= 10;//(uint8_t)TIMESTAMP_LENGTH;
		timestamp->timestamp 		= rte_cpu_to_be_32(rte_rdtsc()/one_usec);
		timestamp->echo_timestamp	= rte_cpu_to_be_32(last_received_timestamp);
	}
}

/*static void parseTcpOptions(struct rte_mbuf *pkt)
{
	uint8_t *option_type = rte_pktmbuf_mtod(pkt, uint8_t *);
	
	if(*option_type == TIMESTAMP_TYPE)
	{
		struct timestamp_option *timestamp = rte_pktmbuf_mtod(pkt, struct timestamp_option *);
		last_received_timestamp = rte_cpu_to_be_32(timestamp->timestamp);
		uint32_t curr_time = rte_rdtsc()/one_usec;		
		calcAverageRtt(abs(curr_time - rte_cpu_to_be_32(timestamp->echo_timestamp)));
		rte_pktmbuf_adj(pkt, 12);
	}
}*/

#endif
