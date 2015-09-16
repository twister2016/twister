#ifndef _IP_H_
#define _IP_H_

#define CHECK_IPv4_CKSUM 0
#define DEFAULT_IPv4_HDR_SIZE 20

extern uint8_t ip_hdr_size;

extern int tw_ip4_packet_parser(struct rte_mbuf *, uint8_t, uint8_t, uint8_t, void *);
extern void tw_ip4_packet_create(struct rte_mbuf *pkt,uint8_t next_port_id,uint32_t src_ip,uint32_t dst_ip,uint16_t length);



#endif
