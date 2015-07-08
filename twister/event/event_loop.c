#include <unistd.h>
#include <rte_lcore.h>
#include <event_loop.h>
#include <eth.h>
#include <rx.h>
#include <tx.h>
#include <queued_pkts.h>

struct event_io * 
reg_io_event(int sock_fd, void * cb_func, uint8_t repeat_event, uint8_t event_flags) {
	int core_id = rte_lcore_id();   //TODO use proper lcore id
	struct event_io * temp_event_io = &root_event_io[core_id];
	if(temp_event_io == NULL) {
		temp_event_io = rte_malloc("struct event_io *", sizeof(struct event_io), 64);
		temp_event_io->event_repeat = repeat_event;
		temp_event_io->event_flags = event_flags;
		temp_event_io->sock_fd = sock_fd;
		temp_event_io->event_cb = cb_func;
		temp_event_io->next = NULL;
		return temp_event_io;
	}
	while(temp_event_io->next != NULL)
		temp_event_io = temp_event_io->next;
	if(event_flags != 0) {
		rte_exit(EXIT_FAILURE,"NO EVENT FLAGS\n"); //TODO end properly
	}
	temp_event_io = rte_malloc("struct event_io *", sizeof(struct event_io), 64);
        temp_event_io->sock_fd = sock_fd;
        temp_event_io->event_cb = cb_func;
        temp_event_io->next = NULL;
	return temp_event_io;
}

int start_io_events(void) {
	int num_rx_pkts, pkt_count,i;
	int core_id = rte_lcore_id();
	struct mbuf_table m[MAX_ETH_PORTS*RX_QUEUES_PER_PORT]; //TODO change to global
	struct rte_mbuf * pkt;
	struct event_io * temp_event = &root_event_io[core_id];
	do {
		printf("\n\n***************\n");
		if(temp_event == NULL) 
			rte_exit(EXIT_FAILURE,"NO EVENT FLAGS\n"); //TODO exit properly
                sleep(1);
                twister_timely_burst();
                update_queued_pkts();
		num_rx_pkts = rx_for_each_queue(m); //TODO for each queue
		if(num_rx_pkts > 0) {
			struct lcore_conf *qconf = &lcore_conf[rte_lcore_id()] ;
			for(i=0;i<qconf->num_queues;i++)
			{
				
					for(pkt_count = 0;pkt_count < m[i].len;pkt_count++) {
					pkt = m[i].m_table[pkt_count];
					if(root_event_io[core_id].event_flags == NO_FLAG_SET) {
										eth_pkt_parser(pkt, m[i].portid); //TODO port id
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
		}
	} while(1); //TODO implement loop ending logic
}


