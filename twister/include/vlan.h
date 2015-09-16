#ifndef _VLAN_H_
#define _VLAN_H_

#include <rte_ether.h>

int tw_vlan_parser(struct rte_mbuf *, uint8_t, uint8_t, void *);


#endif
