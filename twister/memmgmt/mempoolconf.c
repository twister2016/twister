#ifndef _MEMMGMT_C_
#define _MEMMGMT_C_

#include <mempoolconf.h>

uint8_t num_numa_sockets = 1;

int create_rx_tx_mempools(void) {		//--!TODO Have to call "lcore_init" before using this function
	uint8_t numa_socket = 0;
	for(numa_socket=0;numa_socket<num_numa_sockets;numa_socket++) {
		rx_mempool[numa_socket] = rte_mempool_create("rx_mempool_" + numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), \
						rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);	//--!TODO Is mempool name correct--???
		if (&(rx_mempool[numa_socket]) == NULL)
			rte_exit(EXIT_FAILURE, "Cannot init rx mempool on NUMA node %d\n", numa_socket);
		tx_mempool[numa_socket] = rte_mempool_create("tx_mempool_" + numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), \
						rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);
		if (&(tx_mempool[numa_socket]) == NULL)
			rte_exit(EXIT_FAILURE, "Cannot init tx mempool on NUMA node %d\n", numa_socket);
	}
	return 0;
}

int create_queued_pkts_mempools(void) {		//--! To be used by queued packets including pakets waiting for an ARP reply
	uint8_t numa_socket = 0;
        for(numa_socket=0;numa_socket<num_numa_sockets;numa_socket++) {
                queued_pkts_mempool[numa_socket] = rte_mempool_create("queued_pkts_mempool"+numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), \
                                                rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);
                if (&(queued_pkts_mempool[numa_socket]) == NULL)
                        rte_exit(EXIT_FAILURE, "Cannot init queued packets mempool on NUMA node %d\n", numa_socket);
	}
	return 0;
}

#endif
