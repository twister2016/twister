#include <gtpu.h>


static __inline__ void gtpuHdrCtor(struct rte_mbuf * gtpu_payload) 
{
	gtpu_t * gtpu	= gtpu_payload->buf_addr;
	gtpu->flags		= 0x32;
	gtpu->msg_type	= 0xff;
	gtpu->length	= htons( gtpu_payload->data_len );
//	gtpu->teid		= htonl(gtpu_value_in_use->teid);
	gtpu->seqnum	= htons(0x237e);
}

static __inline__ void udpHdrCtor(struct rte_mbuf * gtpu_payload) 
{
	struct udp_hdr * udphdr	= gtpu_payload->buf_addr;
	udphdr->src_port	= htons(default_udp_src);
	udphdr->dst_port	= htons(default_udp_dst);
	udphdr->dgram_len	= htons(gtpu_payload->data_len);
	udphdr->dgram_cksum	= 0;
}

static __inline__ void ipv4HdrCtor(struct rte_mbuf * gtpu_payload) 
{
	struct ipv4_hdr * iphdr	= gtpu_payload->buf_addr;
	uint16_t ipv4_len		= gtpu_payload->data_len;
	iphdr->version_ihl		= (IPv4_VERSION << 4) | (sizeof(struct ipv4_hdr) /4);
	iphdr->type_of_service 	= 0;
	iphdr->total_length		= htons(ipv4_len);
	iphdr->packet_id		= htons(27); //--? any random number
	iphdr->fragment_offset 	= 0;
	iphdr->time_to_live		= 63;
	iphdr->next_proto_id	= UDP_PROTO_ID;
	//printf("enode B IP");
	//printIpInt(gtpu_value_in_use->enodeb_ip);
	iphdr->src_addr			= htonl(gtpu_value_in_use->enodeb_ip);
	iphdr->dst_addr			= htonl(gtpu_value_in_use->sgw_ip);
	//udpChksum((uint16_t *) iphdr);
	//insertIpChksum((uint16_t *) iphdr);
}

int gtpuparsepacket(struct rte_mbuf * pkt)
{
	struct ipv4_hdr * iphdr = rte_pktmbuf_mtod(pkt, gtpu_t * gtpu);
	if (gtpu->teid == htonl(gtpu_value_in_use->teid))
	{
       		rte_pktmbuf_adj(pkt, sizeof(gtpu_t));
        }    
        else
        {
        	return 1;
        }    	
	return 0;
}

/*static uint8_t addGtpuTunnel(struct rte_mbuf * gtpu_payload)
{
	rte_pktmbuf_prepend(gtpu_payload, sizeof(gtpu_t));
	gtpuHdrCtor(gtpu_payload);
	rte_pktmbuf_prepend(gtpu_payload, sizeof(struct udp_hdr));
	udpHdrCtor(gtpu_payload);
	rte_pktmbuf_prepend(gtpu_payload, sizeof(struct ipv4_hdr));
	ipv4HdrCtor(gtpu_payload);
	//l3ToTx(gtpu_value_in_use->sgw_ip, gtpu_payload, ETHER_IP);
	return 0;
}*/
