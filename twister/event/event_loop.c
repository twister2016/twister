#ifndef _EVENT_LOOP_C_
#define _EVENT_LOOP_C_

#include <rte_lcore.h>
#include <event_loop.h>


struct event_io * 
reg_io_event(int sock_fd, void * cb_func, void * cb_args, uint8_t repeat_event, uint8_t event_flags) {
	int core_id = rte_lcore_id();   //TODO use proper lcore id
	struct event_io * temp_event_io = root_event_io[core_id];
	if(root_event_io[core_id] == NULL) {
		root_event_io[core_id] = rte_malloc("struct event_io *", sizeof(struct event_io), 64);
		root_event_io[core_id]->event_repeat = repeat_event;
		root_event_io[core_id]->event_flags = event_flags;
		root_event_io[core_id]->sock_fd = sock_fd;
		root_event_io[core_id]->event_cb = cb_func;
		root_event_io[core_id]->next = NULL;
		return root_event_io[core_id];
	}
	while(temp_event_io->next != NULL)
		temp_event_io = temp_event_io->next;
	if(event_flags != 0) {
		rte_exit(1); //TODO end properly
	}
	temp_event_io = rte_malloc("struct event_io *", sizeof(struct event_io), 64);
        temp_event_io->sock_fd = sock_fd;
        temp_event_io->event_cb = cb_func;
        temp_event_io->next = NULL;
	return temp_event_io;
}

int start_io_events() {
	int num_rx_pkts, pkt_count;
	int core_id = rte_lcore_id();
	struct mbuf_table m;
	struct rte_mbuf * pkt;
	struct event_io * temp_event = root_event_io[core_id];
	do {
		printf("\n\n***************\n");
		if(root_event_io[core_id]) 
			rte_exit(1); //TODO exit properly
                sleep(1);
                twister_timely_burst();
                update_queued_pkts();
		num_rx_pkts = rx_for_each_queue(&m); //TODO for each queue
		if(num_rx_pkts > 0) {
			for(pkt_count = 0;pkt_count < num_rx_pkts;pkt_count++) {
				pkt = m[pkt_count];
				if(root_event_io[core_id].event_flags == NO_FLAG_SET) {
                        			eth_pkt_parser(pkt, 0); //TODO port id
					return 0;
				}
				while (temp_event != NULL) {
					if(temp_event->event_flags == GET_L2_PKTS) {
						printf("Get L2 pkts\n");
						return 0;
					}
					if(temp_event->event_flags == GET_L3_PKTS || temp_event->event_flags == GET_L4_PKTS) {
						printf("Get L3 pkts\n");
						eth_pkt_parser(pkt, 0);
						return 0;
					}
				}
				
			}
		}
	} while(1); //TODO implement loop ending logic
}
/*
int reg_flag_event(uint8_t event_flag, void * event_cb) {
	if(event_loop_flags.flag_event_cb == NULL) {
		if(event_flag == GET_L2_PKTS)
			event_loop_flags.get_l2_packets = 1;
		if(event_flag == GET_L3_PKTS)
			event_loop_flags.get_l3_packets = 1;
	}
}
*/
#endif
