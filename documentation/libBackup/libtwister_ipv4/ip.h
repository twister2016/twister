#ifndef _IP_H_
#define _IP_H_
/**
 * @file
 *
 * IP Helpers in Twister
 */
extern int ip4_packet_parser(struct rte_mbuf *pkt,uint8_t port_id);  /**< ip4_packet_parser - This function parse the IP header fields and forward the packet to UDP parser if the packets is valid for the respective machine. */
extern void ip4_packet_create(struct rte_mbuf *pkt,uint8_t next_port_id,uint32_t src_ip,uint32_t dst_ip,uint16_t length);  /**< ip4_packet_create - This function construct the IP header fields and attach to the packet and forwards the packet to Ethernet constructor. */

#define CHECK_IPv4_CKSUM 0 /**< CHECK_IPv4_CKSUM - This macros defines the IPv4 Checksum equals to 0. This is used to check and forward the packet to dpdk internal IPv4 checksum calculator function.*/
extern uint8_t ip_hdr_size; /**< ip_hdr_size - This Variable contains the IP layer header size of the packet. */


#endif
