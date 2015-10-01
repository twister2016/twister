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

struct tw_buf_s {
	struct rte_mbuf * pkt;
	void * data;
        uint16_t size;
        char * port_name;
};

typedef struct tw_buf_s tw_buf_t;

uint8_t num_numa_sockets ;
struct rte_mempool * rx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * tx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * queued_pkts_mempool[MAX_NUMA_SOCKETS];

int tw_create_rx_tx_mempools(void);
int tw_create_queued_pkts_mempools(void);
inline void tw_free_buffer(tw_buf_t *);
//inline void tw_free(void *);
extern struct rte_mbuf * tw_app_get_buffer(void);
//tw_buf_t * tw_new_buffer(uint16_t);
//void tw_memcpy (void *, const void *, size_t);
inline void tw_free_pkt(tw_buf_t *);

#endif
