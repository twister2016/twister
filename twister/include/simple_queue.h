#ifndef SIMPLE_QUEUE_H
#define SIMPLE_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rte_memcpy.h>
#include <usocket.h>
#include <mempoolconf.h>


#define SQ_NUM_QUEUES  1024

/*
 * structure to store a packet ==> fm_buffer
*/
/*
struct sq_pkt {
	struct sock_conn_t conn;
	struct rte_mbuf * pkt;
    	struct sq_pkt* next_pkt;
};
*/

/*
 * simple link list to store packets as a queue
*/
/*
struct sq_pkt_q {
    uint32_t n_pkts;
    struct sq_pkt* head;
};
*/
struct pkt_queue {
	struct tw_sockaddr_in dst_addr;
	struct rte_mbuf * pkt;
};

struct rte_mbuf_q {
    uint32_t n_pkts;
    struct pkt_queue pkt_queue[MAX_RX_PKT_BURST];
};

extern struct rte_mbuf_q udp_sock_queue[MAX_SOCK_FD];

//extern struct sq_pkt_q soft_q[SQ_NUM_QUEUES]; //TODO make in sync with socket structs
//struct sq_pkt_q udp_socket_q[SQ_NUM_QUEUES];

/*initializes the queues*/
//extern void sq_init(struct sq_pkt_q q_list[]);

/*pushes an item in the beginning of q*/
//extern int sq_push(int q_id, struct sq_pkt_q* q_list, struct rte_mbuf * pkt, struct sock_conn_t conn);

/*pops an item from the end of the queue*/
//extern uint16_t sq_pop(int q_id, struct sq_pkt_q* q_list, void** data, struct sock_conn_t * conn);
//extern int sq_pop(int q_id, struct sq_pkt_q* q_list, void* data, uint16_t size,struct sock_conn_t *conn);

int udp_queue_push(struct rte_mbuf *, uint16_t, struct tw_sockaddr_in);
int udp_queue_pop(uint16_t, struct tw_sockaddr_in *, tw_buf_t *);

#endif  /*simple-queue.h*/
