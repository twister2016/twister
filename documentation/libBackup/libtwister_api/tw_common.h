#ifndef _TWCOMMON_H_
#define _TWCOMMON_H_
/**
 * @file
 *
 * Common Functions in Twister
 */

#include <event_loop.h>


extern int tw_map_port_to_engine(char portName[10], char coreName[10]);/**< map port name tw0 to engine name engine0, engine1 etc */
extern inline void tw_copy_ether_addr(struct ether_addr * s_addr, struct ether_addr * d_addr);/**< copy the source ethernet address to destination ethernet address */
extern struct ether_addr * tw_get_ether_addr(char * portname);/**< retrieve the mac address of portname tw0, tw1 etc */
extern uint8_t tw_convert_str_to_iparr(const char *, uint8_t *);/**< convert ip address string like "192.168.1.10" to usigned int which is used in packets. */
extern uint32_t tw_convert_str_to_hex(char * str, uint8_t length);/**< convert string with given length to hexadecimal */
extern void tw_ip_dec_to_arr( uint32_t ip, uint8_t *arr);/**< convert an unsinged Int Ip address like 16843009 corresponds to 1.1.1.1 to array [1,1,1,1] */
extern uint32_t tw_ip_arr_to_dec(uint8_t *ip);/**< convert ipaddress array [1,1,1,1] to decimal i.e, 16843009*/
extern uint32_t tw_convert_ip_str_to_dec(const char *ip);/**< convert ip string "1.1.1.1" to decimal 16843009  */
extern uint64_t tw_convert_str_to_int(const char *str, uint8_t n);/**< convert a specified number of string characters to int "1212" to 1212 */
extern inline uint16_t tw_be_to_cpu_16(uint16_t);/**< convert big endian to little endian of 2 bytes */
extern inline uint16_t tw_cpu_to_be_16(uint16_t);/**< convert little endian to big endian of 2 bytes*/
extern inline uint32_t tw_cpu_to_be_32(uint32_t);/**< convert little endian to big endian of 4 bytes */
extern uint8_t tw_parse_int_from_string(char*);/**< return an int found in integer */
extern inline int tw_match_port_ether_addr(struct ether_addr *, char * port_name);/**< match the mac-address to a port name returns 1 if true else 0*/
extern inline int tw_is_broadcast_ether_addr(const struct ether_addr * ea);/**< True (1) if the given ethernet address is a broadcast address; false (0) otherwise.  */
extern inline uint16_t tw_ipv4_cksum(struct ipv4_hdr*) ;/**< returns the complemented IPv4 checksum of an IPv4 header. The checksum field must be set to 0 by the caller*/
extern int tw_eth_port_init(void);/**< initialize and configure the all available ethernet ports */
extern int tw_get_port_by_ip(uint32_t ip_addr);/**< get the port number from IP address */
extern int tw_eth_name_to_id(char* portName);/**< convert eth port name like tw0, tw1 to respective id used by twister for indexing the port_conf structure for that port*/
extern void tw_check_all_ports_link(void);/**< returns true if all ports of nic are up otherwise panic occur  */
extern uint32_t tw_get_ip_addr(char *port_name);/**< returns the Ip address of port tw0, tw1 etc */
extern inline void tw_sleep_msec(unsigned);/**< sleep for specified number of milli-seconds */
extern inline void tw_sleep_usec(unsigned);/**< sleep for specified number of micro-seconds */
extern inline void tw_sleep_nsec(unsigned);/**< sleep for specified number of nano-seconds */
extern cJSON * tw_parse_json_file(const char *);/**< parse the JSON file */


#endif
