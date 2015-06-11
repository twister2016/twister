#ifndef _IP_H_
#define _IP_H_

extern int ip4_packet_parser(struct rte_mbuf *pkt,uint8_t port_id);
extern void ip4_packet_create(struct rte_mbuf *pkt,uint8_t next_port_id,uint32_t src_ip,uint32_t dst_ip,uint16_t length);

#define CHECK_IPv4_CKSUM 0



#endif
