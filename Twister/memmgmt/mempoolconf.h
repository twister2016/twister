#ifndef _MEMMGMT_H_
#define _MEMMGMT_H_

#include <rte_mempool.h>
#include <rte_mbuf.h>

#define MAX_NUMA_SOCKETS 2				//--? is there a scenario to have more than 2 NUMA nodes???

#define MBUF_SIZE (2048 + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)
#define NB_MBUF   8192

#define MAX_PKT_BURST 32
#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */


uint8_t num_numa_sockets = 1;
struct rte_mempool * rx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * tx_mempool[MAX_NUMA_SOCKETS];

int create_rx_tx_mempools() {		//--!Have to call "lcore_init" before using this function
	uint8_t numa_socket = 0;
	for(numa_socket=0;numa_socket<num_numa_sockets;numa_socket++) {
		rx_mempool[numa_socket] = rte_mempool_create("rx_mempool_" + numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), \
						rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);
		if (&(rx_mempool[numa_socket]) == NULL)
			rte_exit(EXIT_FAILURE, "Cannot init rx mempool\n");
		tx_mempool[numa_socket] = rte_mempool_create("tx_mempool_" + numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), \
						rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);
		if (&(tx_mempool[numa_socket]) == NULL)
			rte_exit(EXIT_FAILURE, "Cannot init tx mempool\n");
	}
	return 0;
}


#endif
