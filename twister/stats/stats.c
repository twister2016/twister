#include <stats.h>

struct average_filter *root_rtt_average = NULL;
struct average_filter *end_rtt_average = NULL;
const uint8_t average_filter_len = 255;
uint64_t stats_time_period = 0;
uint8_t average_list_size = 0;
int stats_fd = -1;
uint32_t stats_server_ip = 0;
uint16_t l4_stats_port = 0;
uint64_t prev_pkts_rx = 0, prev_pkts_tx = 0;

void tw_clear_scr(void)
{
    const char clr[] = { 27, '[', '2', 'J', '\0' };
    const char topLeft[] = { 27, '[', '1', ';', '1', 'H', '\0' };

    // Clear screen and move to top left
    printf("%s%s", clr, topLeft);
}

int tw_calc_global_stats(void)
{
    tw_stats.secs_passed++;
    tw_stats.rtt = average_rtt;
    tw_stats.rx_pps = tw_stats.pkts_rx - prev_pkts_rx;
    tw_stats.tx_pps = tw_stats.pkts_tx - prev_pkts_tx;
    prev_pkts_rx = tw_stats.pkts_rx;
    prev_pkts_tx = tw_stats.pkts_tx;
	return 0;
}

void tw_print_global_stats(void)
{
    //int proc_engine_id = rte_lcore_id();
    tw_clear_scr();
    printf("****Global Stats****\n");
    printf("Secs Passed %"PRIu64"\nRX PPS %"PRIu64"\nTX PPS %"PRIu64"\nPkts RX %"PRIu64"\nPkts TX %"PRIu64"\n",
           tw_stats.secs_passed, tw_stats.rx_pps, tw_stats.tx_pps, tw_stats.pkts_rx,
           tw_stats.pkts_tx);

}

void tw_calc_average_rtt(uint64_t time_clk)  //TODO multi core verification
{
    struct average_filter *average_entry = (struct average_filter *) rte_malloc(
            "struct average_filter *", sizeof(struct average_filter), 64);
    float curr_pkt_rtt = (float) time_clk / (float) average_filter_len;
    average_entry->timestamp = curr_pkt_rtt;
    average_entry->next = NULL;
    //printf("curr_pkt_rtt %u\n", curr_pkt_rtt);
    if(average_list_size == 0)
    {
        root_rtt_average = average_entry;
        end_rtt_average = average_entry;
        average_rtt = curr_pkt_rtt;
        average_list_size++;
    }
    else if(average_list_size < average_filter_len)
    {
        average_rtt = average_rtt + curr_pkt_rtt;
        end_rtt_average->next = average_entry;
        end_rtt_average = average_entry;
        average_list_size++;
    }
    else
    {
        average_rtt = average_rtt + curr_pkt_rtt;
        end_rtt_average->next = average_entry;
        end_rtt_average = average_entry;
        struct average_filter *temp = root_rtt_average;
        average_rtt = average_rtt - root_rtt_average->timestamp;
        root_rtt_average = root_rtt_average->next;
        rte_free(temp);
    }
}    

