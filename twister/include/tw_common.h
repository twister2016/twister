#ifndef _TWCOMMON_H_
#define _TWCOMMON_H_

#include <event_loop.h>


extern int tw_map_port_to_engine(char portName[10], char coreName[10]);
extern inline void tw_copy_ether_addr(struct ether_addr * s_addr, struct ether_addr * d_addr);
extern struct ether_addr * tw_get_ether_addr(char * portname);
extern uint8_t tw_convert_str_to_iparr(const char *, uint8_t *);
extern uint32_t tw_convert_str_to_hex(char * str, uint8_t length);
extern void tw_ip_dec_to_arr( uint32_t ip, uint8_t *arr);
extern uint32_t tw_ip_arr_to_dec(uint8_t *ip);
extern uint32_t tw_convert_ip_str_to_dec(const char *ip);
extern uint64_t tw_convert_str_to_int(const char *str, uint8_t n);
extern inline uint16_t tw_be_to_cpu_16(uint16_t);
extern inline uint16_t tw_cpu_to_be_16(uint16_t);
extern inline uint32_t tw_cpu_to_be_32(uint32_t);
extern uint8_t tw_parse_int_from_string(char*);
extern inline int tw_match_port_ether_addr(struct ether_addr *, char * port_name);
extern inline int tw_is_broadcast_ether_addr(const struct ether_addr * ea);
extern inline uint16_t tw_ipv4_cksum(struct ipv4_hdr*) ;
extern int tw_eth_port_init(void);
extern int tw_get_port_by_ip(uint32_t ip_addr);
extern int tw_eth_name_to_id(char* portName);
extern void tw_check_all_ports_link(void);
extern uint32_t tw_get_ip_addr(char *port_name);
extern inline void tw_sleep_msec(unsigned);
extern inline void tw_sleep_usec(unsigned);
extern inline void tw_sleep_nsec(unsigned);
extern cJSON * tw_parse_json_file(const char *);


#endif