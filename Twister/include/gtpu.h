#ifndef _GTPU_H_
#define _GTPU_H_

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/queue.h>
#include <net/if.h>
#include <fcntl.h>
#include <setjmp.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <libgen.h>
#include <linux/if_tun.h>
#include <poll.h>
#include <assert.h>

#include <rte_version.h>
#include <rte_config.h>

#include <rte_errno.h>
#include <rte_log.h>
#include <rte_tailq.h>
#include <rte_common.h>
#include <rte_memory.h>
#include <rte_memcpy.h>
#include <rte_memzone.h>
#include <rte_malloc.h>
#include <rte_eal.h>
#include <rte_per_lcore.h>
#include <rte_launch.h>
#include <rte_atomic.h>
#include <rte_cycles.h>
#include <rte_prefetch.h>
#include <rte_lcore.h>
#include <rte_per_lcore.h>
#include <rte_branch_prediction.h>
#include <rte_pci.h>
#include <rte_random.h>
#include <rte_timer.h>
#include <rte_debug.h>
#include <rte_ether.h>
#include <rte_ethdev.h>
#include <rte_ring.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_tcp.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include <udp.h>

static gtpu_values_t *gtpu_value_in_use = NULL;
static uint16_t default_udp_src = 0x0868;
static uint16_t default_udp_dst = 0x0868;
#define IPv4_VERSION    4


typedef struct {
	uint32_t	teid;
	uint32_t	ue_ip;
	uint32_t	sgw_ip;
	uint32_t	enodeb_ip;
} __attribute__ ((__packed__)) gtpu_values_t;



typedef struct gtpu {
	uint8_t		flags;
	uint8_t 	msg_type;
	uint16_t 	length; //including GTP-U header
	uint32_t 	teid;
	uint16_t 	seqnum;
	uint16_t	buffer;
} __attribute__ ((__packed__)) gtpu_t;




#endif
