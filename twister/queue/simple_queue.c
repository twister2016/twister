#include <stdio.h>
#include <stdint.h>
#include <rte_mbuf.h>
#include <rte_malloc.h>
#include <simple_queue.h>

struct sq_pkt_q udp_socket_q[SQ_NUM_QUEUES];
/*initializes the queues*/
void sq_init(struct sq_pkt_q q_list[])
{
    	int i = 0;
    	for (; i<SQ_NUM_QUEUES; i++) {
        	q_list[i].n_pkts = 0;
        	q_list[i].head = NULL;
    	}
}

/*
* pushes an item at the end of q
* does not free data
* returns 1 if success
*/
int sq_push(int q_id, struct sq_pkt_q* q_list, struct rte_mbuf * pkt, struct sock_conn_t conn)
{
	printf("sq_push\n");
    	struct sq_pkt * new_pkt = NULL, *pkt_p;
    	if((new_pkt = (struct sq_pkt*) rte_malloc("struct sq_pkt *", sizeof(struct sq_pkt), 64)) == NULL) {
		return 0;
	}
	new_pkt->conn=conn;
    	new_pkt->pkt = pkt;
    	q_list[q_id].n_pkts++;
    	new_pkt->next_pkt = NULL;
    	if(q_list[q_id].head == NULL) {
        	q_list[q_id].head = new_pkt;
        	return 1;
    	}
    	pkt_p = q_list[q_id].head;
    	while(pkt_p->next_pkt != NULL)
        	pkt_p = pkt_p->next_pkt;
    	pkt_p->next_pkt = new_pkt;
    	return 1;
}
/*
* pops a packet from q_id index of q_list
* assumes data has been allocated 'size' by caller
* returns 0 if no data otherwise returns size of received data 
*/
int sq_pop(int q_id, struct sq_pkt_q* q_list, void* data, struct sock_conn_t * conn)
{
	struct sq_pkt* tmp_pkt = q_list[q_id].head;
	
	if(tmp_pkt == NULL)
		return 0;
	if(q_list[q_id].n_pkts < 1)
		return 0;
	struct rte_mbuf * temp_mbuf = tmp_pkt->pkt;
	int payload_size = temp_mbuf->data_len;
	void * pkt_payload = rte_pktmbuf_mtod(temp_mbuf, void *);

	data = rte_malloc("void *", sizeof(payload_size), RTE_CACHE_LINE_SIZE);
	rte_memcpy(data, pkt_payload, payload_size);
	conn->src_port = tmp_pkt->conn.src_port;
	conn->dst_port = tmp_pkt->conn.dst_port;
	conn->src_ip = tmp_pkt->conn.src_ip;
	conn->dst_ip = tmp_pkt->conn.dst_ip;
	q_list[q_id].head = tmp_pkt->next_pkt;
	q_list[q_id].n_pkts--;
	rte_pktmbuf_free(tmp_pkt->pkt);
	free(tmp_pkt);
	return payload_size;
}

