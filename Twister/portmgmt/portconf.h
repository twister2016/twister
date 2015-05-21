#ifndef _PORTCONF_H_
#define _PORTCONF_H_

#define MAX_ETH_PORTS 16

#define ACCEPT_BRDCAST	1		//If the ports should accept the broadcast packets or not

enum {
	REPLY_ARP		= 0x00000001,
	REPLY_ICMPv4	= 0x00000002	//--!TODO add support for icmpv4 and v6
} __attribute__((__packed__));


struct port_info {
	union {
		uint32_t	ip_addr;
		uint8_t		ip[4];
	}
	uint8_t	flags;
	struct ether_addr * eth_mac;		//--!TODO initialize eth_mac in init func
	uint16_t vlan_tag;
	//struct port_stats;	//--! add stats info
};

struct port_info port_info[MAX_ETH_PORTS];

#endif