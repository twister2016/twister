#ifndef _UDP_H_
#define _UDP_H_
/**
 * @file
 *
 * UDP Helpers in Twister
 */
#define UDP_PROTO_ID 17 /**< UDP_PROTO_ID - This macro defines the Proto ID of UDP which is 17. */
#define TCP_PROTO_ID 06  /**< TCP_PROTO_ID - This macro defines the Proto ID of TCP which is 06. */

struct sock_conn_t
{
	uint16_t src_port; 
	uint16_t dst_port; 
	uint32_t dst_ip; 
	uint32_t dst_ip; 
	uint16_t l4_proto;
};

extern void udp_packet_parser(struct rte_mbuf *pkt, uint32_t src_ip, uint32_t dst_ip); /**< udp_packet_parser - This function parse the UDP packet header and push the packet to respective socket queue . */
extern void udp_packet_create(struct rte_mbuf *pkt, struct sock_conn_t *udp_conn);  /**< udp_packet_create - This function construct the UDP header and attach it to the payload of the packet and forward the packet to IP layer constructor. */
extern uint8_t udp_hdr_size; /**< udp_hdr_size - This variable contains the UDP header size. */
extern uint8_t tcp_hdr_size;  /**< tcp_hdr_size - This variable contains the TCP header size. */

#endif
