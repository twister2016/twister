#include <rte_udp.h>
#include <rte_memcpy.h>
#include <rte_byteorder.h>
#include <rte_mbuf.h>

#include <portconf.h>
#include <usocket.h>
#include <udp.h>
#include <ip.h>
#include <event_loop.h>

void udp_packet_parser(struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip, uint8_t prev_hdr_size, uint8_t processing_flag, void * cb_func) {
	uint16_t dst_port, src_port;
	struct udp_hdr *udp_hdr_d = rte_pktmbuf_mtod(pkt, struct udp_hdr *) + prev_hdr_size; //--??
	dst_port  = rte_be_to_cpu_16(udp_hdr_d->dst_port);
	src_port = rte_be_to_cpu_16(udp_hdr_d->src_port);	
	int temp_port = 0;
   	struct socket_info * sockptr = NULL;
    	for (temp_port=0;temp_port<=maxfd;temp_port++){			//TODO see if code can be improved ito performance
        	sockptr =&sockets[temp_port];
        	if(sockptr->port==dst_port){
            		break;
            	}
    	}	
	if (temp_port >= maxfd){
		rte_pktmbuf_free(pkt);
		return;
	}
	if(processing_flag == LOOP_PROCESS) {
		if(sockets[temp_port].type == TW_SOCK_RAW) {
			switch(sockets[temp_port].proto) {
				case(TW_IPPROTO_UDP): 	//pass the UDP layer and payload
					rte_pktmbuf_adj(pkt, prev_hdr_size);
					break;
				case(TW_IPPROTO_IP): 	//pass the IP layer and payload
					rte_pktmbuf_adj(pkt, prev_hdr_size - DEFAULT_IPv4_HDR_SIZE);
					break;
				case(TW_IPPROTO_RAW):	//pass the whole packet including Eth layer
					break;
				default:
					break;
			}
		}
		else		//pass only the payload of UDP
			rte_pktmbuf_adj(pkt, sizeof(struct udp_hdr) + prev_hdr_size);
		add_packet_to_udp_queue(temp_port,pkt,src_ip,dst_ip,src_port,dst_port);
	}
	else
	{
		void (*udp_cb_func) (int, struct rte_mbuf *, uint32_t, uint32_t, uint16_t, uint16_t) = cb_func;
		udp_cb_func(temp_port,pkt,src_ip,dst_ip,src_port,dst_port);
	}
}

void udp_packet_create(struct rte_mbuf *pkt, struct sock_conn_t *udp_conn)
{
	struct udp_hdr *udp_hdr_d;
	rte_pktmbuf_prepend(pkt, sizeof(struct udp_hdr));
	udp_hdr_d = rte_pktmbuf_mtod(pkt, struct udp_hdr *);
	udp_hdr_d->src_port = rte_cpu_to_be_16(udp_conn->src_port);
	udp_hdr_d->dst_port = rte_cpu_to_be_16(udp_conn->dst_port);
	udp_hdr_d->dgram_len = rte_cpu_to_be_16(pkt->pkt_len);
	//udp_hdr_d->dgram_cksum = udp_hdr_d->src_port + udp_hdr_d->dst_port + pkt->pkt_len ;
	udp_hdr_d->dgram_cksum = 0;
	ip4_packet_create(pkt, UDP_PROTO_ID,udp_conn->src_ip,udp_conn->dst_ip,pkt->pkt_len);
}

