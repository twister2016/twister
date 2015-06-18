#include <rte_mbuf.h>
#include <pktctor.h>

#include <rte_ether.h>
#include <udp.h>
#include <portconf.h>
#include <vlan.h>
#include <eth.h>
#include <arplogic.h>
#include <ip.h>
#include <queued_pkts.h>


int pkt_ctor(struct rte_mbuf* m, void* ptr_l4, uint32_t eth_proto) {
    struct udp_conn_t *udp_conn = ( struct udp_conn_t * )ptr_l4 ;
    if (m == NULL) {
        return -1;
    }
    
    if (eth_proto == UDP_PROTO_ID ) {
        udp_packet_create(m, udp_conn);
        
    }
    
    
    
return 0;
}
