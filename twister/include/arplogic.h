#ifndef _ARPLOGIC_H_
#define _ARPLOGIC_H_

#include <rte_arp.h>
#include <rte_malloc.h>
#include <rte_ether.h>
#include <tx.h>
#include <portconf.h>
#include <tw_api.h>

#define ARP_ENTRY_TIMEOUT 15000
#define NUM_ARP_RETRIES 3


struct ether_addr broadcastmac; 
struct arp_table * arp_table_root;
uint32_t arp_table_size;

int tw_parse_arp(struct rte_mbuf *, uint8_t);
int tw_send_arp_reply(struct rte_mbuf *, uint8_t);
int tw_process_arp_reply(struct rte_mbuf* pkt, uint8_t);
int tw_add_arp_entry(uint32_t ip_to_add, struct ether_addr, uint8_t);
#endif
