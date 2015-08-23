#ifndef _MEMMGMT_H_
#define _MEMMGMT_H_

#include <rte_mempool.h>
#include <rte_malloc.h>
#include <rte_mbuf.h>

#define MAX_NUMA_SOCKETS 2				//--? is there a scenario to have more than 2 NUMA nodes???

#define MBUF_SIZE (2048 + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)
#define NB_MBUF   8192

#define MAX_RX_PKT_BURST 32
#define MAX_TX_PKT_BURST 32

struct tw_buf_s {
	struct rte_mbuf * pkt;
	void * data;
};

typedef struct tw_buf_s tw_buf_t;

uint8_t num_numa_sockets ;
struct rte_mempool * rx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * tx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * queued_pkts_mempool[MAX_NUMA_SOCKETS];

int create_rx_tx_mempools(void);
int create_queued_pkts_mempools(void);
inline void tw_free_no_tx(tw_buf_t *);
inline void tw_free(void *);
extern struct rte_mbuf *app_get_buffer(void);
tw_buf_t * tw_new_buffer(uint16_t);

#endif
