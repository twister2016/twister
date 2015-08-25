#ifndef _VLAN_H_
#define _VLAN_H_
/**
 * @file
 *
 * VLAN Helpers in Twister
 */
#include <rte_ether.h>
int vlan_parser(struct rte_mbuf *, uint8_t); /**< vlan_parser - This function parse the VLAN packets and forward it to respective next callback. */
int vlan_ctor(struct rte_mbuf *, uint8_t, uint16_t); /**< vlan_ctor - This function constructs the VLAN packets, if VLAN traffic is selected in the configuration of the applications and forward it to respective next callback. */

#endif
