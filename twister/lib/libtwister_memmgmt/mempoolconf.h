#ifndef _MEMMGMT_H_
#define _MEMMGMT_H_
/**
 * @file
 *
 * Memory pool configurations Helpers in Twister
 */
#include <rte_mempool.h>
#include <rte_malloc.h>
#include <rte_mbuf.h>

#define MAX_NUMA_SOCKETS 2				//--? is there a scenario to have more than 2 NUMA nodes???

#define MBUF_SIZE (2048 + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)
#define NB_MBUF   8192

#define MAX_PKT_BURST 32

uint8_t num_numa_sockets ;
struct rte_mempool * rx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * tx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * queued_pkts_mempool[MAX_NUMA_SOCKETS];

int create_rx_tx_mempools(void); /**< create_rx_tx_mempools - This functions create the RX and RX memory pools. This function is called in the start of the application. Calling the initfuncs automatically call and reserve the memory pools. */
int create_queued_pkts_mempools(void); /**< create_queued_pkts_mempools - This functions create the memory pool for the queued packets. This function is called in the start of the application. Calling the initfuncs automatically call and reserve the memory pool for queued packets. */
extern struct rte_mbuf *app_get_buffer(void);  /**< app_get_buffer - This function when called return a new allocated rte_mbuf type packet to the caller. */

#endif
