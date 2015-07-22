#include <unistd.h>
#include <rte_lcore.h>
#include <event_loop.h>
#include <eth.h>
#include <rx.h>
#include <tx.h>
#include <queued_pkts.h>


struct event_io * 
reg_io_event(int sock_fd, void * cb_func, uint8_t repeat_event, uint8_t event_flags, uint8_t event_type) {
	int core_id = rte_lcore_id();   //TODO modify if launch_one_lcore calling position changes
	struct event_io * temp_event_io;
	
	temp_event_io = root_event_io[core_id];
	if(temp_event_io == NULL) {
		temp_event_io = rte_malloc("struct event_io *", sizeof(struct event_io), RTE_CACHE_LINE_SIZE);
		root_event_io[core_id] = temp_event_io;
	}
	else {
		while(temp_event_io->next != NULL)
			temp_event_io = temp_event_io->next;
		//if(event_flags != 0) 
			//rte_exit(EXIT_FAILURE,"EVENT FLAG SETTING NOT ALLOWED\n"); //TODO end properly
		temp_event_io->next = rte_malloc("struct event_io *", sizeof(struct event_io), RTE_CACHE_LINE_SIZE);
		temp_event_io = temp_event_io->next;
	}
	if(temp_event_io == NULL)
		rte_exit(EXIT_FAILURE,"CAN'T ALLOCATE IO EVENT\n");
	temp_event_io->type = event_type;
	temp_event_io->repeat = repeat_event;
	temp_event_io->flags = event_flags;
        temp_event_io->sock_fd = sock_fd;
        temp_event_io->event_cb = cb_func;
        temp_event_io->next = NULL;
	return temp_event_io;
}

int start_io_events(uint32_t secs_to_run) {
	uint8_t infinite_loop = 0, continue_loop = 1;
	if (secs_to_run == INFINITE_LOOP)
		infinite_loop = 1;
	int num_rx_pkts, pkt_count, payload_size = 0, i;
	global_pps_delay = 10000000000/global_pps_limit;
	int core_id = rte_lcore_id();
	uint64_t curr_time_cycle = 0, prev_stats_cycle= 0, prev_stats_calc = 0, prev_queue_cycle = 0, time_diff = 0;
	uint64_t loop_start_time = get_current_timer_cycles();
	struct lcore_conf *qconf = &lcore_conf[rte_lcore_id()];
	struct mbuf_table m[qconf->num_queues];
	struct rte_mbuf * pkt;
	struct event_io * temp_event;
	struct sock_conn_t conn;
	void * payload_data = NULL;
	void (*cb_func_with_flags) (struct rte_mbuf *, uint8_t);
	void (*rx_cb_func) (int, void *, int, struct sock_conn_t);
	void (*tx_cb_func) (int);

	do {
		printf("\n**********event loop***********\n");
		curr_time_cycle = get_current_timer_cycles();
		time_diff = get_time_diff(curr_time_cycle, prev_queue_cycle, one_msec);
		printf("time diff %lu\n", time_diff);
		if(unlikely(time_diff > queue_update_limit)) {
			printf("update queued pkts\n");
                	update_queued_pkts(curr_time_cycle);
			prev_queue_cycle = curr_time_cycle;
		}
		
		time_diff = get_time_diff(curr_time_cycle, prev_stats_calc, one_msec);
		if(unlikely(time_diff > stats_calc_limit)) {
			calc_global_stats();
			prev_stats_calc = curr_time_cycle;
		}

		time_diff = get_time_diff(curr_time_cycle, prev_stats_cycle, one_msec);
		printf("time diff %lu, stat update limit %d\n", time_diff, stats_update_limit);
		if(unlikely(time_diff > stats_update_limit)) {
			printf("send stats pkts\n");
			send_stats_pkt();
			print_global_stats();
			prev_stats_cycle = curr_time_cycle;
		}

		temp_event = root_event_io[core_id];

		if(temp_event == NULL) 
			rte_exit(EXIT_FAILURE,"NO EVENTS REGISTERED\n"); 
                sleep(1);
                twister_timely_burst();
		num_rx_pkts = rx_for_each_queue(m);
		if(num_rx_pkts > 0) {	//TODO Enable rx and tx check
			for(i=0;i<qconf->num_queues;i++)
			{
				for(pkt_count = 0;pkt_count < m[i].len;pkt_count++) {
					pkt = m[i].m_table[pkt_count];	
					if(event_flags_global == GET_L2_PKTS) {
						cb_func_with_flags = temp_event->event_cb;
						cb_func_with_flags(pkt, m[i].portid);
					}
					else {
						eth_pkt_parser(pkt, m[i].portid);
					}
				}
			}
		}
		if(event_flags_global == NO_FLAG_SET) {
			while(temp_event != NULL) {
				if(temp_event->type == RX_CALLBACK) {
					rx_cb_func = temp_event->event_cb;
					while(udp_socket_q[temp_event->sock_fd].n_pkts > 0) {	//Implemented for UDP sockets only
						payload_size = sq_pop(temp_event->sock_fd, udp_socket_q, &payload_data, &conn);
						rx_cb_func(temp_event->sock_fd, payload_data, payload_size, conn);
					}
				}
				else { //TX_CALLBACK
					tx_cb_func = temp_event->event_cb;
					tx_cb_func(temp_event->sock_fd);
				}
				temp_event = temp_event->next;
			}
		}
		if(!infinite_loop) {
			if(get_time_diff(curr_time_cycle, loop_start_time, one_sec) >= secs_to_run)
				continue_loop = 0;
		}
	} while(continue_loop);

	return 0;
}


