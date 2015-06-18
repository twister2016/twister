#ifndef _UDP_H_
#define _UDP_H_

#define UDP_PROTO_ID	17
struct udp_conn_t
{
	uint16_t src_port;
	uint16_t dst_port;
	uint32_t src_ip;
	uint32_t dst_ip;

};
extern void udp_packet_parser(struct rte_mbuf *, uint32_t,uint32_t );
extern void udp_packet_create(struct rte_mbuf *, struct udp_conn_t *);

#endif
