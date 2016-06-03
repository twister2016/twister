#include <tx.h>
#include <stdint.h>

#define drain_tsc (rte_get_tsc_hz() + US_PER_S - 1) / US_PER_S * BURST_TX_DRAIN_US

static int tw_send_burst(struct lcore_conf *qconf, unsigned n, uint8_t port)
{
    struct rte_mbuf **m_table;
    uint64_t ret;
    m_table = (struct rte_mbuf **) qconf->tx_mbufs[port].m_table;
    ret = rte_eth_tx_burst(port, /*(uint16_t) queueid*/0, m_table, (uint16_t) n);
    //tw_stats.pkts_tx += ret; //global variable in stats.h
    tw_stats.pkts_tx = tw_stats.pkts_tx + ret;
    //printf("before prnting = %llu  ret = %llu\n", tw_stats.pkts_tx, ret);
    //printf("tw_stats pkt tx = %I64d ret=%u\n",ret);
    //exit(1) ;
    if(unlikely(ret < n))
    {
        do
        {
            rte_pktmbuf_free(m_table[ret]);
        }
        while (++ret < n);
        tw_stats.pkts_dropped += (n - ret);
    }

    return 0;
}

int tw_timely_burst(void)
{

    unsigned portid, cur_tsc, diff_tsc, lcore_id;
    struct lcore_conf *qconf;

    lcore_id = rte_lcore_id();
    qconf = &lcore_conf[lcore_id];
    cur_tsc = rte_rdtsc();

    /*
     * TX burst queue drain
     */
    diff_tsc = cur_tsc - prev_tsc;
    if(unlikely(diff_tsc > drain_tsc))
    {
        for(portid = 0; portid < qconf->num_port; portid++)
        {
            if(qconf->tx_mbufs[portid].len == 0)
                continue;
            tw_send_burst(&lcore_conf[lcore_id], qconf->tx_mbufs[portid].len, (uint8_t) portid);
            qconf->tx_mbufs[portid].len = 0;
            prev_tsc = cur_tsc;
        }
    }
    return 0;
}

int tw_add_pkt_to_tx_queue(struct rte_mbuf * m, uint8_t port)
{
    unsigned lcore_id, len;
    struct lcore_conf *qconf;

    lcore_id = rte_lcore_id();

    qconf = &lcore_conf[lcore_id];
    len = qconf->tx_mbufs[port].len;
    qconf->tx_mbufs[port].m_table[len] = m;
    len++;
    /* enough pkts to be sent */
    if(unlikely(len == MAX_TX_PKT_BURST))
    {
        tw_send_burst(qconf, MAX_TX_PKT_BURST, port);
        len = 0;
    }

    qconf->tx_mbufs[port].len = len;
    return 0;

}

