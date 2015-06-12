#include <stats.h>

int init_stats (uint8_t port_id, uint32_t dst_ip ) {
    
    uint8_t socket_id = rte_eth_dev_socket_id(port_id);
    struct rte_mbuf * m = rte_pktmbuf_alloc ( tx_mempool[socket_id] );
    rte_pktmbuf_prepend(m, sizeof(struct stats));
	struct stats *sta  = rte_pktmbuf_mtod(m, struct stats *);
    sta->PPS = 512; //randomly written PPS packet persecond
    sta->timestamp = rte_cpu_to_be_32(rte_rdtsc()/one_usec);
    
    struct udp_conn_t* udp_stat_struct = rte_malloc ("UdpStructurForStat", sizeof (struct udp_conn_t ), 0 );
    udp_stat_struct->src_port = port_id;
    udp_stat_struct->dst_port = l4_stat_port;             //randomly chosen for destination app written in python
    udp_stat_struct->src_ip = port_info[port_id].start_ip_addr;
    udp_stat_struct->dst_ip = dst_ip;
    pkt_ctor(m,udp_stat_struct ,UDP_PROTO_ID);
    
    return 0;
}
    
    















































