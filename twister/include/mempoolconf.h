/** @file All memory management functions
 *
 */

#ifndef _MEMMGMT_H_
#define _MEMMGMT_H_

#include <rte_mempool.h>
#include <rte_malloc.h>
#include <rte_mbuf.h>
#include <rte_memcpy.h>

#define MAX_NUMA_SOCKETS 2				//--? is there a scenario to have more than 2 NUMA nodes???

#define MBUF_SIZE (2048 + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)
#define NB_MBUF   8192

#define MAX_RX_PKT_BURST 32
#define MAX_TX_PKT_BURST 32

/** Stores any packet and contains info about the packet
 *
 */
struct tw_buf_s
{
        struct rte_mbuf * pkt;
        void * data;
        uint16_t size;
        char * port_name;
};

typedef struct tw_buf_s tw_buf_t;

uint8_t num_numa_sockets;
struct rte_mempool * rx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * tx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * queued_pkts_mempool[MAX_NUMA_SOCKETS];

/** Calls another function, just for smoke test.
 *
 * @return 0(TRUE), -1(FALSE)
 */
int tw_smoke_create_rx_tx_mempools(void);

/** Creates the memory pools that give space to buffers
 *
 * @return 0(TRUE)
 */
int tw_create_rx_tx_mempools(void);

/** Frees the memory used by m_buf structures
 *
 * @param ptr :Pointer to buffer to free
 *
 */
inline void tw_free_buffer(tw_buf_t * ptr);

struct rte_mbuf * tw_app_get_buffer(void);

#endif
