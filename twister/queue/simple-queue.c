#include <stdio.h>
#include <stdint.h>
#include <rte_mbuf.h>
#include <simple-queue.h>

struct sq_pkt_q soft_q[SQ_NUM_QUEUES];
/*initializes the queues*/
void sq_init(struct sq_pkt_q q_list[])
{
    int i = 0;
    for (; i<SQ_NUM_QUEUES; i++){
        q_list[i].n_pkts = 0;
        q_list[i].head = NULL;
    }
}

/*
* pushes an item in the beginning of q
* does not free data
* returns 1 if success
*/
int sq_push(int q_id, struct sq_pkt_q* q_list, void* data, uint16_t size, struct udp_conn_t conn)
{
    struct sq_pkt* new_pkt = NULL, *pkt_p;
    if((new_pkt = (struct sq_pkt*) malloc(sizeof(struct sq_pkt))) == NULL){
		return 0;
	 }
    new_pkt->data = data;
	new_pkt->size=size;
	new_pkt->conn=conn;
    q_list[q_id].n_pkts++;
    new_pkt->next_pkt = NULL;
    if(q_list[q_id].head == NULL){
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
/*
int sq_pop(int q_id, struct sq_pkt_q* q_list, void* data, uint16_t size,struct udp_conn_t *conn)
{
    struct sq_pkt* tmp_pkt = q_list[q_id].head;
    int pkt_size = 0;
  
    if(tmp_pkt == NULL)
        return 0;
  
    if(q_list[q_id].n_pkts < 1)
      return 0;
  
    if(tmp_pkt->size > size){
        printf("[WARN] Popped pkt's payload size is greater than the \
                     buffer provided. So returning.\n"); 
        return 0;
    }
    memcpy(data, tmp_pkt->data, tmp_pkt->size);
    pkt_size = tmp_pkt->size;
    q_list[q_id].head = tmp_pkt->next_pkt;
    q_list[q_id].n_pkts--;
    free(tmp_pkt->data);
    free(tmp_pkt);
    return pkt_size;
}
*/
