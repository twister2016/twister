#include <stdint.h>
#include <stdlib.h>
#include <rte_mbuf.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include <usocket.h>
#include <udp.h>
#include <rte_ether.h>
#include <mempoolconf.h>
#include <simple_queue.h>

int maxfd=0;
struct socket_info sockets[1024];
int udp_socket(uint32_t ip_addr,uint32_t port)
{	
	int i=0;
	struct socket_info * sockptr = NULL;
	for (i=0;i<maxfd;i++)
	{
		sockptr =&sockets[i];
		if(sockptr->port==port)
		{
			return -1;
		}
	}
	sockptr =&sockets[maxfd];
	sockptr->ip_addr=ip_addr;
	sockptr->port=port;
	maxfd++;
	return maxfd-1;
}

int udp_send(int sockfd, void * buffer, uint16_t buf_len, uint16_t total_payload_len, uint32_t dst_addr, uint16_t port)
{
	struct rte_mbuf *pkt=app_get_buffer();
	//rte_pktmbuf_append(pkt,buf_len + sizeof(struct udp_hdr ) + sizeof(struct ipv4_hdr) + 40);
	//rte_pktmbuf_trim(pkt,buf_len + sizeof(struct udp_hdr ) + sizeof(struct ipv4_hdr) + 40);
	struct socket_info * sockptr = NULL;
	sockptr =&sockets[sockfd];
	struct sock_conn_t dummy;
	dummy.src_port=sockptr->port;
	dummy.dst_port=port;
	dummy.src_ip=sockptr->ip_addr;
	dummy.dst_ip=dst_addr;
	if(total_payload_len < buf_len)
		total_payload_len = buf_len;
	if(total_payload_len > MAX_UDP_PAYLOAD)
		total_payload_len = MAX_UDP_PAYLOAD;
	rte_pktmbuf_append(pkt, total_payload_len);
	void * payload = rte_pktmbuf_mtod(pkt, void *);
	rte_memcpy(payload, buffer, buf_len);
	//pkt->userdata=buffer;
	//pkt->data_len=buf_len;
	//pkt->pkt_len=pkt->data_len;
	udp_packet_create(pkt,&dummy);
	return 0;
}

void add_packet_to_udp_queue(int sock_fd, struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip,uint16_t dst_port,uint16_t src_port){
	struct sock_conn_t dummy;
	dummy.src_port = src_port;
	dummy.dst_port = dst_port;
	dummy.src_ip = src_ip;
	dummy.dst_ip = dst_ip;
	sq_push(sock_fd, udp_socket_q, pkt, dummy);
}

int udp_recv(int sock_fd, void * buffer, struct sock_conn_t * conn)
{
	return (sq_pop(sock_fd, udp_socket_q, buffer, conn));
}

