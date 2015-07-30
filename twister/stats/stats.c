#include <stats.h>
 
struct average_filter *root_rtt_average = NULL;
struct average_filter *end_rtt_average = NULL;
const uint8_t average_filter_len = 255;
uint64_t stats_time_period=0;
uint8_t average_list_size = 0;
int stats_fd = -1;
uint32_t stats_server_ip = 0;
uint16_t l4_stats_port = 0;
uint64_t prev_pkts_rx = 0, prev_pkts_tx = 0;
uint64_t global_pps_limit = 0;
uint64_t global_pps_delay = 0;

void clear_scr(void)
{
        const char clr[] = { 27, '[', '2', 'J', '\0' };
        const char topLeft[] = { 27, '[', '1', ';', '1', 'H','\0' };

        // Clear screen and move to top left
        printf("%s%s", clr, topLeft);
}

int open_stats_socket(uint32_t server_ip, uint16_t server_port) {
	stats_server_ip = server_ip;
	l4_stats_port = server_port;
	stats_fd = udp_socket(port_info[stats_port].start_ip_addr, l4_stats_port);
	return stats_fd;
}

int send_stats_pkt(void) {
	if(stats_server_ip) {  //if stats_server_ip is not zero
		global_stats_option.timestamp = get_current_timer_cycles();
		udp_send(stats_fd, (void *) &global_stats_option, sizeof(struct stats_option), sizeof(struct stats_option), stats_server_ip, l4_stats_port);
	}
	return 0;
}

int calc_global_stats(void) {
	global_stats_option.secs_passed++;
	global_stats_option.rtt = average_rtt;
	global_stats_option.rx_pps = global_stats_option.pkts_rx - prev_pkts_rx;
	global_stats_option.tx_pps = global_stats_option.pkts_tx - prev_pkts_tx;
	prev_pkts_rx = global_stats_option.pkts_rx;
	prev_pkts_tx = global_stats_option.pkts_tx;
	//if(global_pps_delay > 10) {
		float change = ((float)global_pps_limit - global_stats_option.tx_pps)/global_pps_limit;
		if(global_pps_delay == 0)
			global_pps_delay = 10;
		if(change != 0)
			global_pps_delay -= (change * global_pps_delay);
	//}
	if(global_stats_option.secs_passed <= 5) //TODO check if this logic is required
	{
		global_pps_delay = global_pps_delay * 5;
	}
	return 0;
}

void print_global_stats(void) {
	//clear_scr();
	printf("****Global Stats****\n");
	printf("Secs Passed %lu\nRX PPS %lu\nTX PPS %lu\nPkts RX %lu\nPkts TX %lu\nRTT %lu\n", global_stats_option.secs_passed, global_stats_option.rx_pps, global_stats_option.tx_pps, global_stats_option.pkts_rx, global_stats_option.pkts_tx, global_stats_option.rtt);
}

void calc_average_rtt(uint64_t time_clk)
{			
	struct average_filter *average_entry = (struct average_filter *) rte_malloc("struct average_filter *", sizeof(struct average_filter), 64);
	float curr_pkt_rtt = (float)time_clk/(float)average_filter_len;
		
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

