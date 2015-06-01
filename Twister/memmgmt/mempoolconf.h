#ifndef _MEMMGMT_H_
#define _MEMMGMT_H_

#include <rte_mempool.h>
#include <rte_mbuf.h>

#define MAX_NUMA_SOCKETS 2				//--? is there a scenario to have more than 2 NUMA nodes???

#define MBUF_SIZE (2048 + sizeof(struct rte_mbuf) + RTE_PKTMBUF_HEADROOM)
#define NB_MBUF   8192

#define MAX_PKT_BURST 32

uint8_t num_numa_sockets = 1;
struct rte_mempool * rx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * tx_mempool[MAX_NUMA_SOCKETS];
struct rte_mempool * queued_pkts_mempool[MAX_NUMA_SOCKETS];

int create_rx_tx_mempools() {		//--!TODO Have to call "lcore_init" before using this function
	uint8_t numa_socket = 0;
	for(numa_socket=0;numa_socket<num_numa_sockets;numa_socket++) {
		rx_mempool[numa_socket] = rte_mempool_create("rx_mempool_" + numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), \
						rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);	//--!TODO Is mempool name correct--???
		if (&(rx_mempool[numa_socket]) == NULL)
			rte_exit(EXIT_FAILURE, "Cannot init rx mempool on NUMA node %d\n", numasocket);
		tx_mempool[numa_socket] = rte_mempool_create("tx_mempool_" + numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), \
						rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);
		if (&(tx_mempool[numa_socket]) == NULL)
			rte_exit(EXIT_FAILURE, "Cannot init tx mempool on NUMA node %d\n", numasocket);
	}
	return 0;
}

int create_queued_pkts_mempools() {		//--! To be used by queued packets including pakets waiting for an ARP reply
	uint8_t numa_socket = 0;
        for(numa_socket=0;numa_socket<num_numa_sockets;numa_socket++) {
                queued_pkts_mempool[numa_socket] = rte_mempool_create("queued_pkts_mempool"+numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), \
                                                rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);
                if (&queued_pkts_mempool([numa_socket]) == NULL)
                        rte_exit(EXIT_FAILURE, "Cannot init queued packets mempool on NUMA node %d\n", numasocket);
	}
	return 0;
}

#endif
