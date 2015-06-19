#ifndef _STATS_H_
#define _STATS_H_

#include <rte_memcpy.h>
#include <rte_byteorder.h>
#include <rte_mbuf.h>
#include <rte_udp.h>
#include <rte_malloc.h>
#include <portconf.h>
#include <ip.h>
#include <udp.h>
#include <eth.h>
#include <rte_ethdev.h>
#include <portconf.h>
#include <rte_cycles.h>
#include <pktctor.h>
#include <timerdefs.h>
#define l4_stat_port 5555
                     //TODO take input from user and set a variable here, If user wanted to set a variable or not?

extern uint8_t stats_status;
extern uint64_t stats_time_period;




struct average_filter
{	float   timestamp;
struct average_filter* next;
} __attribute__ ((__packed__));

extern struct average_filter *root_rtt_average ;
extern struct average_filter *end_rtt_average ;
extern const uint8_t average_filter_len ;


extern uint8_t average_list_size;

struct print_stats
{
uint64_t rx_pps;
uint64_t tx_pps;
uint64_t total_pps;
uint64_t pkt_sent;
uint64_t pkt_recvd;
float rx_gbps;
float tx_gbps;
float total_gbps;
float rtt;
float one_way_time;
struct print_stats *next;
} __attribute__ ((__packed__));

extern struct print_stats *root_test_stats ;
extern struct print_stats *end_test_stats ;


struct packet_stats{
uint64_t timestamp;
uint64_t PPS;
uint64_t packet_received;
uint64_t packet_transmitted;
uint64_t packet_dropped;
uint64_t latency;

} __attribute__ ((__packed__));

struct packet_stats global_stats;

void clearScr(void);
int init_stats (uint8_t, uint32_t );
void printXfgenStats(void);
void calcAverageRtt(uint64_t);
void writeTestStats(void);

uint64_t prev_pkt_transmitted;
uint64_t prev_pkt_received;
extern struct rte_eth_stats* port_stats;

float   average_rtt; 
uint64_t data_pkt_recvd;
uint64_t data_pkt_sent;
uint32_t seconds_passed;   
uint8_t l4_proto ; //TODO init at run time, if protocl used is to be udp or tcp
uint32_t PKT_PAYLOAD_SIZE;//TODO init decide payload size    

extern uint32_t test_runtime ; //TODO init file, initate the testruntime
extern uint64_t pps_limit ; //TODO
extern uint64_t pps_delay ; //TODO


#endif
