#include <unistd.h>
#include <rte_lcore.h>
#include <event_loop.h>
#include <eth.h>
#include <rx.h>
#include <tx.h>
#include <queued_pkts.h>


/*struct event_io * 
reg_io_event(int sock_fd, void * cb_func, uint8_t repeat_event, uint8_t event_flags, uint8_t event_type) {
	int core_id = rte_lcore_id();
	struct event_io * temp_event_io;
	
	temp_event_io = root_event_io[core_id];
	if(temp_event_io == NULL) {
		temp_event_io = rte_malloc("struct event_io *", sizeof(struct event_io), RTE_CACHE_LINE_SIZE);
		root_event_io[core_id] = temp_event_io;
	}
	else {
		while(temp_event_io->next != NULL)
			temp_event_io = temp_event_io->next;
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
*/

int tw_loop_init(tw_loop_t * event_loop) { //TODO secs_to_run for  event loop
	event_loop->data = NULL;
	int core_id = rte_lcore_id();
	if(global_pps_limit[core_id])
		global_pps_delay[core_id] = 10000000000/global_pps_limit[core_id];
	else
		global_pps_delay[core_id] = 0;
	return 0;
}

int tw_loop_stop(tw_loop_t * event_loop) {
	event_loop->stop_flag = 0;
	return 0;
}

tw_loop_t * tw_default_loop(void) {
	tw_loop_t * loop = rte_malloc("tw_loop_t *", sizeof(tw_loop_t), RTE_CACHE_LINE_SIZE);
	return loop;
}

int tw_udp_init(tw_loop_t * loop, tw_udp_t * udp_handle) {
		
	tw_handle_t * temp_handle = loop->handle_queue;
        if(temp_handle == NULL) {
                temp_handle = rte_malloc("tw_udp_t", sizeof(tw_udp_t), RTE_CACHE_LINE_SIZE);
                loop->handle_queue = temp_handle;
        }
        else {
                while(temp_handle->next != NULL)
                        temp_handle = temp_handle->next;
                temp_handle->next = rte_malloc("tw_udp_t", sizeof(tw_udp_t), RTE_CACHE_LINE_SIZE);
                temp_handle = temp_handle->next;
        }
	
	//TODO populate tw_loop_t entries if needed
	temp_handle = (tw_handle_t *) udp_handle;
	loop->active_handles++;
	return 0;
}

int tw_udp_bind(tw_udp_t * udp_handle, struct tw_sockaddr_in * addr, uint8_t flags) {
	udp_handle->sock_fd = udp_socket(addr->sock_ip, addr->sock_port); //TODO standardize
	sockets[udp_handle->sock_fd].type = flags; //TODO assign properly
	return 0;
}

struct tw_sockaddr_in * tw_ip4_addr(char * ip_str, uint16_t l4_port) {
	struct tw_sockaddr_in * addr = rte_malloc("struct tw_sockaddr_in", sizeof(struct tw_sockaddr_in), RTE_CACHE_LINE_SIZE);
	uint32_t ip_addr = convert_ip_str_to_dec(ip_str);
	addr->sock_ip = ip_addr;
	addr->sock_port = l4_port;
	return addr;
}


int tw_loop_run(tw_loop_t * event_loop) {
	
	uint8_t infinite_loop = 0, continue_loop = 1;
	if (event_loop->secs_to_run == INFINITE_LOOP)
		infinite_loop = 1;
	int num_rx_pkts = 0, pkt_count = 0, i;
	//int  payload_size = 0;
	//int core_id = rte_lcore_id();
	uint64_t curr_time_cycle = 0, prev_queued_pkts_cycle = 0, prev_stats_calc = 0, time_diff = 0;
	uint64_t loop_start_time = get_current_timer_cycles();
	struct lcore_conf *qconf = &lcore_conf[rte_lcore_id()];
	struct mbuf_table m[qconf->num_queues];
	struct rte_mbuf * pkt;
	//struct sock_conn_t conn;
	//void * payload_data = NULL;
	tw_handle_t * temp_handle_queue;
	//void (*cb_func_with_flags) (struct rte_mbuf *, uint8_t);
	//void (*rx_cb_func) (int, void *, uint16_t, struct sock_conn_t);
	//void (*tx_cb_func) (int);

	do {
		if(event_loop->stop_flag)
			return 0;
		curr_time_cycle = get_current_timer_cycles();
		time_diff = get_time_diff(curr_time_cycle, prev_queued_pkts_cycle, one_msec);
		if(unlikely(time_diff > queue_update_limit)) {
                	update_queued_pkts(curr_time_cycle);
			prev_queued_pkts_cycle = curr_time_cycle;
		}
		
		time_diff = get_time_diff(curr_time_cycle, prev_stats_calc, one_msec);
		if(unlikely(time_diff > stats_calc_limit)) {
			calc_global_stats();
			prev_stats_calc = curr_time_cycle;
		}
		/*
		time_diff = get_time_diff(curr_time_cycle, prev_stats_cycle, one_msec);
		if(unlikely(time_diff > stats_update_limit)) {
			send_stats_pkt();
			print_global_stats();
			prev_stats_cycle = curr_time_cycle;
		}
		*/
		//temp_event = root_event_io[core_id];

		temp_handle_queue = event_loop->handle_queue;
		if(temp_handle_queue == NULL) {
			printf("No Event Handles Registered\n");
			return -1;
		}
		
		if(qconf->core_tx) //If TX for this core is enabled
			twister_timely_burst();
		
		if(qconf->core_rx) //If RX for this core is enabled
			num_rx_pkts = rx_for_each_queue(m);

		if(num_rx_pkts > 0) {
			for(i=0;i<qconf->num_queues;i++) {
				for(pkt_count = 0;pkt_count < m[i].len;pkt_count++) {
					pkt = m[i].m_table[pkt_count];	
					eth_pkt_parser(pkt, m[i].portid, LOOP_PROCESS, NULL);
				}
			}
		}
		/*
		while(temp_handle_queue != NULL) {
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
		*/
		
		if(!infinite_loop) {
			if(get_time_diff(curr_time_cycle, loop_start_time, one_sec) >= event_loop->secs_to_run)
				continue_loop = 0;
		}
	} while(continue_loop);

	return 0;
}

/*
int start_io_events(uint32_t secs_to_run) {
	uint8_t infinite_loop = 0, continue_loop = 1;
	if (secs_to_run == INFINITE_LOOP)
		infinite_loop = 1;
	int num_rx_pkts = 0, pkt_count = 0, payload_size = 0, i;
	if(global_pps_limit)
		global_pps_delay = 10000000000/global_pps_limit;
	else
		global_pps_delay = 0;
	printf("global_pps_limit %lu, global_pps_delay %lu", global_pps_limit, global_pps_delay);
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
	void (*rx_cb_func) (int, void *, uint16_t, struct sock_conn_t);
	void (*tx_cb_func) (int);

	do {
		curr_time_cycle = get_current_timer_cycles();
		time_diff = get_time_diff(curr_time_cycle, prev_queue_cycle, one_msec);
		if(unlikely(time_diff > queue_update_limit)) {
                	update_queued_pkts(curr_time_cycle);
			prev_queue_cycle = curr_time_cycle;
		}
		
		time_diff = get_time_diff(curr_time_cycle, prev_stats_calc, one_msec);
		if(unlikely(time_diff > stats_calc_limit)) {
			calc_global_stats();
			prev_stats_calc = curr_time_cycle;
		}

		time_diff = get_time_diff(curr_time_cycle, prev_stats_cycle, one_msec);
		if(unlikely(time_diff > stats_update_limit)) {
			send_stats_pkt();
			print_global_stats();
			prev_stats_cycle = curr_time_cycle;
		}

		temp_event = root_event_io[core_id];

		if(temp_event == NULL) 
			rte_exit(EXIT_FAILURE,"NO EVENTS REGISTERED\n"); 
                //sleep(1);
                twister_timely_burst();
		if(qconf->core_rx) //If RX for this core is enabled
			num_rx_pkts = rx_for_each_queue(m);

		if(num_rx_pkts > 0) {
			for(i=0;i<qconf->num_queues;i++) {
				for(pkt_count = 0;pkt_count < m[i].len;pkt_count++) {
					pkt = m[i].m_table[pkt_count];	
					if(unlikely(temp_event->type != RX_CALLBACK)) {
						rte_exit(EXIT_FAILURE,"FIRST EVENT SHOULD BE AN RX EVENT\n");
					}
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

		if(event_flags_global == NO_FLAG_SET) { //Multiple RX and TX events can be registered in this case
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
		else { //If event_flags_global is set for L2, L3 or L4 pkts
			if(likely(temp_event->type != TX_CALLBACK)) {
				temp_event = root_event_io[core_id]->next;
			}
			while(temp_event != NULL) { //Multiple TX events can be registered
				tx_cb_func = temp_event->event_cb;
				tx_cb_func(temp_event->sock_fd);
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
*/
