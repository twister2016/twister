#include <unistd.h>
#include <rte_lcore.h>
#include <event_loop.h>
#include <eth.h>
#include <rx.h>
#include <tx.h>
#include <queued_pkts.h>

int tw_loop_init(tw_loop_t * event_loop) { //TODO secs_to_run for  event loop
	event_loop->data = NULL;
	int core_id = rte_lcore_id();
	if(global_pps_limit[core_id])
		global_pps_delay[core_id] = 10000000000/global_pps_limit[core_id];
	else
		global_pps_delay[core_id] = 0;
	return 0;
}

int tw_stop(tw_loop_t * event_loop) {
	event_loop->stop_flag = 0;
	return 0;
}

tw_loop_t * tw_default_loop(uint16_t time_to_run) {
	tw_loop_t * loop = rte_malloc("tw_loop_t *", sizeof(tw_loop_t), RTE_CACHE_LINE_SIZE);
	loop->secs_to_run = time_to_run;
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
	udp_handle->addr = NULL;
	udp_handle->sock_fd = udp_socket(addr->sock_ip, addr->sock_port);
	sockets[udp_handle->sock_fd].type = TW_SOCK_DGRAM;
	sockets[udp_handle->sock_fd].proto = TW_IPPROTO_UDP;
	udp_handle->flags = flags;
	return 0;
}

struct tw_sockaddr_in * tw_ip4_addr(char * ip_str, uint16_t l4_port) {
	struct tw_sockaddr_in * addr = rte_malloc("struct tw_sockaddr_in", sizeof(struct tw_sockaddr_in), RTE_CACHE_LINE_SIZE);
	uint32_t ip_addr = convert_ip_str_to_dec(ip_str);
	addr->sock_ip = ip_addr;
	addr->sock_port = l4_port;
	return addr;
}

int tw_udp_recv_start(tw_udp_t * udp_handle, void * alloc_cb, void * recv_cb) {
	if(udp_handle == NULL)
		return -1;
	udp_handle->alloc_cb = alloc_cb;
	udp_handle->recv_cb = recv_cb;
	return 0;
}

int tw_run(tw_loop_t * event_loop) {
	
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