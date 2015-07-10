#include <unistd.h>
#include <rte_lcore.h>
#include <event_loop.h>
#include <eth.h>
#include <rx.h>
#include <tx.h>
#include <queued_pkts.h>


struct event_io * 
reg_io_event(int sock_fd, void * cb_func, uint8_t repeat_event, uint8_t event_flags) {
	printf("reg_io_event with callback %p root event %p\n", cb_func, root_event_io[rte_lcore_id()]);
	int core_id = rte_lcore_id();   //TODO use proper lcore id
	struct event_io * temp_event_io;
	
	temp_event_io = root_event_io[core_id];
	if(temp_event_io == NULL) {
		temp_event_io = rte_malloc("struct event_io *", sizeof(struct event_io), RTE_CACHE_LINE_SIZE);
		root_event_io[core_id] = temp_event_io;
		printf("event regd at root\n");
	}
	else {
		while(temp_event_io->next != NULL)
			temp_event_io = temp_event_io->next;
		if(event_flags != 0) 
			rte_exit(EXIT_FAILURE,"EVENT FLAG SETTING NOT ALLOWED\n"); //TODO end properly
		temp_event_io->next = rte_malloc("struct event_io *", sizeof(struct event_io), RTE_CACHE_LINE_SIZE);
		temp_event_io = temp_event_io->next;
		printf("event regd at temp\n");
	}
	if(temp_event_io == NULL)
		rte_exit(EXIT_FAILURE,"CAN'T ALLOCATE IO EVENT\n");
	temp_event_io->event_repeat = repeat_event;
	temp_event_io->event_flags = event_flags;
        temp_event_io->sock_fd = sock_fd;
        temp_event_io->event_cb = cb_func;
        temp_event_io->next = NULL;
	return temp_event_io;
}

int start_io_events(void) {	//TODO tx callbacks --???
	int num_rx_pkts, pkt_count, payload_size = 0, i;
	int core_id = rte_lcore_id();
	struct lcore_conf *qconf = &lcore_conf[rte_lcore_id()] ;
	struct mbuf_table m[qconf->num_queues];
	struct rte_mbuf * pkt;
	struct event_io * temp_event;
	struct sock_conn_t conn;
	void * payload_data = NULL;
	void (*cb_func_with_flags) (struct rte_mbuf *, uint8_t);
	void (*cb_func) (void *, int, struct sock_conn_t);
	do {
		temp_event = root_event_io[core_id];
		printf("\n\n*******eventloop core %d********\n", core_id);

		if(temp_event == NULL) 
			rte_exit(EXIT_FAILURE,"NO EVENTS REGISTERED\n"); 
                //sleep(1);
		printf("twister_timely_burst\n");
                twister_timely_burst();
                update_queued_pkts();
		printf("update_queued_pkts");
		num_rx_pkts = rx_for_each_queue(m);
		printf("event loop total pkts rx %d\n", num_rx_pkts);
		if(num_rx_pkts > 0) {
			for(i=0;i<qconf->num_queues;i++)
			{
				for(pkt_count = 0;pkt_count < m[i].len;pkt_count++) {
					pkt = m[i].m_table[pkt_count];	
					if(temp_event->event_flags == GET_L2_PKTS) {
						printf("Get L2 pkts\n");
						cb_func_with_flags = temp_event->event_cb;
						cb_func_with_flags(pkt, m[i].portid);
					}
					else {
						eth_pkt_parser(pkt, m[i].portid);
						printf("eth_pkt_parser");
					}
				}
			}
		}
		if(temp_event->event_flags == NO_FLAG_SET) {
			while(temp_event != NULL) {
				cb_func = temp_event->event_cb;
				printf("cb_func %p in temp event %p root event %p root next %p sockfd %d\n", temp_event->event_cb, temp_event, root_event_io[rte_lcore_id()], root_event_io[rte_lcore_id()]->next, temp_event->sock_fd);
				while(udp_socket_q[temp_event->sock_fd].n_pkts > 0) {	//Implemented for UDP sockets only
					payload_size = sq_pop(temp_event->sock_fd, udp_socket_q, &payload_data, &conn);
					printf("payload data %s, %d\n", (char *) payload_data, payload_size);
					cb_func(payload_data, payload_size, conn);
					printf("CB end");
				}
				temp_event = temp_event->next;
			}
		}
		
	} while(1); //TODO implement loop ending logic
}

