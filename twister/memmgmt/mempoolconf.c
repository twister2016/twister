#include <mempoolconf.h>
#include <tw_api.h>

uint8_t num_numa_sockets = 1; //TODO generalize it

int tw_create_rx_tx_mempools(void)
{		//--!TODO Have to call "lcore_conf_init" before using this function
    uint8_t numa_socket = 0;
    char rx_mempool_name[15];
    char tx_mempool_name[15];
    for(numa_socket = 0; numa_socket < num_numa_sockets; numa_socket++)
    {
        //printf("%d socket id %d NB_MBUF, %lu MBUF_SIZE, %d numa_socket\n", rte_socket_id(), NB_MBUF, MBUF_SIZE, numa_socket);
        //sprintf(rx_mempool_name, "rx_mempool_%u", numa_socket);
        rx_mempool[numa_socket] = rte_mempool_create(rx_mempool_name, NB_MBUF, MBUF_SIZE, 32,
                                                     sizeof(struct rte_pktmbuf_pool_private),
                                                     rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init,
                                                     NULL, rte_socket_id(), 0);	//--!TODO Is 32 the best cache align val???
        if(rx_mempool[numa_socket] == NULL)
            rte_exit(EXIT_FAILURE, "Cannot init rx mempool on NUMA node %d\n", numa_socket);
        //sprintf(tx_mempool_name, "tx_mempool_%u", numa_socket);
        tx_mempool[numa_socket] = rte_mempool_create(tx_mempool_name, NB_MBUF, MBUF_SIZE, 32,
                                                     sizeof(struct rte_pktmbuf_pool_private),
                                                     rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init,
                                                     NULL, rte_socket_id(), 0);
        if(tx_mempool[numa_socket] == NULL)
            rte_exit(EXIT_FAILURE, "Cannot init tx mempool on NUMA node %d\n", numa_socket);
    }
    return 0;
}

tw_buf_t * tw_new_buffer(uint16_t buffer_size)
{
    struct rte_mbuf * pkt = tw_app_get_buffer();
    if(buffer_size)
        rte_pktmbuf_append(pkt, buffer_size);

    tw_buf_t * tw_buff = rte_malloc("tw_buf_t *", sizeof(tw_buf_t), RTE_CACHE_LINE_SIZE);
    tw_buff->size = buffer_size;
    tw_buff->pkt = pkt;
    tw_buff->data = rte_pktmbuf_mtod(pkt, void *);
    return tw_buff;
}

void tw_memcpy(void * dst, const void * src, size_t n)
{
    rte_memcpy(dst, src, n);
    return;
}

struct rte_mbuf * tw_app_get_buffer(void)
{
    return rte_pktmbuf_alloc(tx_mempool[0]); //TODO remove hardcoded value '0' and use NUMA socket id
}

inline void tw_free(void * ptr)
{
    rte_free(ptr);
}

inline void tw_free_buffer(tw_buf_t * ptr)
{
    rte_pktmbuf_free(ptr->pkt);
    rte_free((void *) ptr);
    return;
}

inline void tw_free_pkt(tw_buf_t * ptr)
{
    rte_pktmbuf_free(ptr->pkt);
    return;
}

void * tw_malloc(char *type, uint32_t size)
{
    return rte_malloc(type, size, RTE_CACHE_LINE_SIZE);
}
/*int create_queued_pkts_mempools(void) {		//--! To be used by queued packets including pakets waiting for an ARP reply
 uint8_t numa_socket = 0;
 for(numa_socket=0;numa_socket<num_numa_sockets;numa_socket++) {
 queued_pkts_mempool[numa_socket] = rte_mempool_create("queued_pkts_mempool" + numa_socket, NB_MBUF, MBUF_SIZE, 32, sizeof(struct rte_pktmbuf_pool_private), rte_pktmbuf_pool_init, NULL, rte_pktmbuf_init, NULL, numa_socket, 0);
 if (queued_pkts_mempool[numa_socket] == NULL)
 rte_exit(EXIT_FAILURE, "Cannot init queued packets mempool on NUMA node %d\n", numa_socket);
 }
 return 0;
 }
 */
