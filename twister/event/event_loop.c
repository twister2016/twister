#include <unistd.h>
#include <rte_lcore.h>
#include <stats.h>
#include <eth.h>
#include <rx.h>
#include <tx.h>
#include <queued_pkts.h>
#include <tw_api.h>

#define ICMP_PROTO_ID	1

int Printing_Enable = 1;
int tw_loop_init(tw_loop_t * event_loop) { //TODO secs_to_run for  event loop
    event_loop->data = NULL;
    if (global_pps_limit)
        global_pps_delay = 10000000000 / global_pps_limit;
    else
        global_pps_delay = 0;
    return 0;
}

int tw_stop(tw_loop_t * event_loop) {
    event_loop->stop_flag = 0;
    return 0;
}

tw_loop_t * tw_default_loop(uint16_t time_to_run) {
    tw_loop_t * loop = rte_malloc("tw_loop_t *", sizeof (tw_loop_t), RTE_CACHE_LINE_SIZE);
    loop->secs_to_run = time_to_run;
    return loop;
}

tw_rx_t * tw_rx_init(tw_loop_t * loop) {
    tw_rx_t * temp_rx_handle = loop->rx_handle_queue;
    //udp_handle = loop->rx_handle_queue;

    if (temp_rx_handle == NULL) {
        temp_rx_handle = rte_malloc("tw_udp_t *", sizeof (tw_udp_t), RTE_CACHE_LINE_SIZE);
        loop->rx_handle_queue = temp_rx_handle;
    } else {
        while (temp_rx_handle->next != NULL)
            temp_rx_handle = temp_rx_handle->next;
        temp_rx_handle->next = rte_malloc("tw_udp_t *", sizeof (tw_udp_t), RTE_CACHE_LINE_SIZE);
        temp_rx_handle = temp_rx_handle->next;
    }

    //TODO populate tw_loop_t entries if needed
    //udp_handle = temp_udp_handle;
    temp_rx_handle->handle_type = TW_UDP_HANDLE;
    //loop->active_handles++;
    //udp_handle = temp_udp_handle;
    //udp_handle->flags = 0;
    //return 0;
    return temp_rx_handle;
}

int tw_rx_start(tw_rx_t * rx_handle, void * recv_cb) {
    if (rx_handle == NULL)
        return -1;
    rx_handle->recv_cb = recv_cb;
    return 0;
}

tw_tx_t * tw_tx_init(tw_loop_t * loop) {
    tw_tx_t * temp_handle = loop->tx_handle_queue;
    if (temp_handle == NULL) {
        temp_handle = rte_malloc("tw_tx_t *", sizeof (tw_tx_t), RTE_CACHE_LINE_SIZE);
        loop->tx_handle_queue = temp_handle;
    } else {
        while (temp_handle->next != NULL)
            temp_handle = temp_handle->next;
        temp_handle->next = rte_malloc("tw_tx_t *", sizeof (tw_tx_t), RTE_CACHE_LINE_SIZE);
        temp_handle = temp_handle->next;
    }
	
    //tx_handle = (tw_tx_t *) temp_handle;
    temp_handle->handle_type = TW_TX_HANDLE;
    loop->active_handles++;
    return temp_handle;
}

int tw_tx_start(tw_tx_t * tx_handle, void * tx_cb) {
    if (tx_handle == NULL)
        return -1;

    tx_handle->tx_cb = tx_cb;
    return 0;
}

struct icmp_echo* ICMP;

tw_timer_t * tw_timer_init(tw_loop_t * loop) {
    tw_timer_t * temp_handle = loop->timer_handle_queue;
    if (temp_handle == NULL) {
        temp_handle = rte_malloc("tw_timer_t *", sizeof (tw_timer_t), RTE_CACHE_LINE_SIZE);
        loop->timer_handle_queue = temp_handle;
    } else {
        while (temp_handle->next != NULL)
            temp_handle = temp_handle->next;
        temp_handle->next = rte_malloc("tw_timer_t *", sizeof (tw_timer_t), RTE_CACHE_LINE_SIZE);
        temp_handle = temp_handle->next;
    }

    //TODO populate tw_loop_t entries if needed
    //timer_handle = (tw_timer_t *) temp_handle;
    temp_handle->handle_type = TW_TIMER_HANDLE;
    loop->active_handles++;
    return temp_handle;
}

/*int tw_timer_bind(tw_timer_t * timer_handle, struct tw_sockaddr_in * addr, int sock_fd, uint8_t flags) {
    timer_handle->flags = flags;
    timer_handle->sock_fd = sock_fd;
    timer_handle->dst_addr = addr;
    return 0;
}*/

int tw_timer_start(tw_timer_t* timer_handle, tw_timer_cb timer_cb, uint64_t timeout) {
    if (timer_handle == NULL)
        return -1;
    timer_handle->timer_cb = timer_cb;
    timer_handle->timeout = (timeout) *( rte_get_tsc_hz() / 1000 );
    return 0;
}

int tw_run(tw_loop_t * event_loop) {
    uint64_t stats_calc_lim = stats_calc_limit* rte_get_tsc_hz();
    uint8_t continue_loop = 1;
    int num_rx_pkts = 0, pkt_count = 0, i;
    uint64_t curr_time_cycle = 0, prev_stats_calc = 0, time_diff = 0;
    struct lcore_conf *qconf = &lcore_conf[rte_lcore_id()];
    struct mbuf_table m[qconf->num_port];
    struct rte_mbuf * pkt;
    tw_buf_t temp_buffer;
    struct ether_hdr* eth;
    uint16_t eth_type;
    //struct ipv4_hdr * ipHdr_d;
    //uint32_t dst_ip,src_ip;
 
    tw_rx_t * temp_rx_handle;
    tw_tx_t * temp_tx_handle;
    tw_timer_t * temp_timer_handle;

    void (*tw_rx_cb) (tw_rx_t *, tw_buf_t *);
    void (*tw_tx_cb) (tw_tx_t *);
    void (*tw_timer_cb) (tw_timer_t *);

    do {
       /* if (event_loop->stop_flag)
            return 0;*/
        curr_time_cycle = tw_get_current_timer_cycles();
		
        tw_timely_burst();
	time_diff = (curr_time_cycle - prev_stats_calc);
        if (unlikely(time_diff > stats_calc_lim)) {
            tw_calc_global_stats();
            prev_stats_calc = curr_time_cycle;
        }
        temp_rx_handle = event_loop->rx_handle_queue;
        if (temp_rx_handle != NULL)
            num_rx_pkts = tw_rx_for_each_queue(m);
        if (num_rx_pkts > 0) {
            for (i = 0; i < qconf->num_port; i++) {
                for (pkt_count = 0; pkt_count < m[i].len; pkt_count++) {
                    pkt = m[i].m_table[pkt_count];
label:
		    eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
    		    eth_type = rte_be_to_cpu_16(eth->ether_type);
		    switch (eth_type) {
			case ETHER_TYPE_VLAN:
				rte_vlan_strip(pkt);
				goto label;
            		case ETHER_TYPE_ARP:
                		tw_parse_arp(pkt, i);
                		break;
			case ETHER_TYPE_IPv4:
				temp_buffer.pkt = pkt;
                    		temp_buffer.data = rte_pktmbuf_mtod(pkt, tw_buf_t *);
                    		temp_buffer.size = pkt->data_len;
                    		temp_buffer.port_name = port_info[i].port_name;
/*				ipHdr_d = (struct ipv4_hdr *) ((uint8_t*)temp_buffer.data + sizeof(struct ether_hdr));
				printf("ipHdr_d->next_proto_id == ICMP_PROTO_ID %02x == %02x\n", ipHdr_d->next_proto_id , ICMP_PROTO_ID);
				if (ipHdr_d->next_proto_id == ICMP_PROTO_ID) {
                    			ICMP=(struct icmp_echo*)((uint8_t*)temp_buffer.data + sizeof(struct ether_hdr) + sizeof(struct ipv4_hdr));
                    			ICMP->type=0;//for icmp reply type is zero
                    			ICMP->checksum=0;
                    			ICMP->checksum= tw_calcsum((unsigned short*)ICMP, sizeof(struct icmp_echo));
           	    			dst_ip  = (ipHdr_d->dst_addr);
                    			src_ip = (ipHdr_d->src_addr);
                    			ipHdr_d->dst_addr = (src_ip);
                    			ipHdr_d->src_addr = (dst_ip);
                    			ipHdr_d->hdr_checksum = tw_ipv4_cksum(ipHdr_d);
                    			tw_copy_ether_addr(&(eth->s_addr), &(eth->d_addr));
                			tw_copy_ether_addr(port_info[i].eth_mac, &(eth->s_addr));
                    			printf("ICMP\n");
					tw_add_pkt_to_tx_queue(pkt, i);
                		}
*/
                    		tw_rx_cb = temp_rx_handle->recv_cb;
                    		tw_rx_cb(temp_rx_handle, &temp_buffer);
				break;

			}
                }
            }
        }
        if (event_loop->active_handles > 0) {
            temp_tx_handle = event_loop->tx_handle_queue;
            while (temp_tx_handle != NULL) {
                tw_tx_cb = temp_tx_handle->tx_cb;
                tw_tx_cb(temp_tx_handle);
                temp_tx_handle->last_run_time = curr_time_cycle;
                temp_tx_handle = temp_tx_handle->next;
            }
            
            temp_timer_handle = event_loop->timer_handle_queue;
            while (temp_timer_handle != NULL) {
                time_diff = (curr_time_cycle - temp_timer_handle->last_run_time);
                if (unlikely(time_diff > temp_timer_handle->timeout)) {
                    tw_timer_cb = temp_timer_handle->timer_cb;
                    tw_timer_cb(temp_timer_handle); 
                    temp_timer_handle->last_run_time = curr_time_cycle;
                }
                temp_timer_handle = temp_timer_handle->next;
            }
        }



    } while (continue_loop);

    return 0;
}
