#include <rte_memcpy.h>
#include <rte_byteorder.h>
#include <rte_mbuf.h>
#include <rte_udp.h>
#include <rte_malloc.h>
#include <portconf.h>
#include <ip.h>
#include <udp.h>
#include <eth.h>
#include <portconf.h>
#include <rte_cycles.h>
#include <pktctor.h>
#include <timerdefs.h>
#define l4_stat_port 5555
struct stats{
    uint32_t timestamp;
	uint32_t PPS;
	
} __attribute__ ((__packed__));

int init_stats (uint8_t port_id, uint32_t dst_ip );
    
    











































