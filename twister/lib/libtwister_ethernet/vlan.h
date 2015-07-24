#ifndef _VLAN_H_
#define _VLAN_H_
/**
 * @file
 *
 * VLAN Helpers in Twister
 */
#include <rte_ether.h>



/**********************************************/
int vlan_parser(struct rte_mbuf *, uint8_t);
/*********************************************/
/**********************************************/
int vlan_ctor(struct rte_mbuf *, uint8_t, uint16_t);
/*********************************************/

#endif
