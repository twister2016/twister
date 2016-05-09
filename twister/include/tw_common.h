#ifndef _TWCOMMON_H_
#define _TWCOMMON_H_

#include <event_loop.h>
/**
 * @file
 *
 * Common Functions in Twister
 */
 
/** Copy the Ethernet address from source to destination
* @param s_addr
*   A pointer to a ether_addr structure holding the Ethernet address to copy. 
* @param d_addr
*   A pointer to a ether_addr structure where to copy the Ethernet address.
*/
inline void tw_copy_ether_addr(struct ether_addr * s_addr, struct ether_addr * d_addr);

/** retrieve the mac address of portname tw0, tw1 etc
* @param portname
*   A string "tw0", "tw1", whose mac-address is to be retreived . 
* @return
*   A pointer to a ether_addr structure containing the ethernet-address.
*/
inline struct ether_addr * tw_get_ether_addr(char * portname);

/** convert ip string to unsigned int 32-bit which is sent over the wire i.e, "1.1.1.1" to decimal 16843009.
* @param ip
*   A string of valid IP address <x.x.x.x>. 
* @return
*   Unsigned 32bit Integer corresponding to IPv4 string.
*/
uint32_t tw_convert_ip_str_to_dec(const char *ip);

/** Co a 16-bit value from big endian to CPU order.
*/
inline uint16_t tw_be_to_cpu_16(uint16_t);

/** Co a 32-bit value from big endian to CPU order.
*/
inline uint32_t tw_be_to_cpu_32(uint32_t);

/** Co a 16-bit value from CPU order to Big endian.
*/
inline uint16_t tw_cpu_to_be_16(uint16_t);

/** Co a 32-bit value from CPU order to Big endian.
*/
inline uint32_t tw_cpu_to_be_32(uint32_t);

/** return an int found in integer.
*/
uint8_t tw_parse_int_from_string(char*);

/** match the mac-address to a port name.
* @param eth_addr
*   pointer to a mac-address
* @port_name
*   A char pointer with portname string <tw0> <tw1> etc.
* @return
*   TRUE(1), False(0)
*/
inline int tw_match_port_ether_addr(struct ether_addr * eth_addr, char * port_name);

/** Check if mac address is broadcast or not.
* @param eth_addr
*   pointer to a mac-address
* @return
*   TRUE(1) if mac-address is broadcast, False(0) otherwise
*/
inline int tw_is_broadcast_ether_addr(const struct ether_addr * ea);

/** Calculate the checksum of IPv4 Header.
* @param ipv4_hdr
*   pointer to a an IPv4-address.
* @return
*   Unsigned 32-bit Integer of calculated checksum.
*/
inline uint16_t tw_ipv4_cksum(struct ipv4_hdr*) ;

/** get the port number from IP address.
* @param ip_addr
*   Integer corresponding to that port, A port Id.
*/
int tw_get_port_by_ip(uint32_t ip_addr);

/** convert eth port name like tw0, tw1 to respective id used by twister for indexing the port_conf structure for that port
* @param portName
*   A char pointer with portname string <tw0> <tw1> etc.
* @return
*   Integer corresponding to that port, A port Id.
*/
int tw_eth_name_to_id(char* portName);

/** check the link of all available ports
* @return
*  TRUE if all ports of nic are up otherwise panic occur 
*/
void tw_check_all_ports_link(void);

/** returns the subnet mask against port tw0, tw1 etc
* @param portName
*   A char pointer with portname string <tw0> <tw1> etc.
* @return
*   An unsigned 32-bit Ipv4 address
*/
uint32_t tw_get_gateway_ip(char *portName);
/** returns the gateway address of port tw0, tw1 etc
* @param portName
*   A char pointer with portname string <tw0> <tw1> etc.
* @return
*   An unsigned 32-bit Ipv4 address
*/
uint32_t tw_get_subnet_mask(char *portName);
/** returns the Ip address of port tw0, tw1 etc
* @param portName
*   A char pointer with portname string <tw0> <tw1> etc.
* @return
*   An unsigned 32-bit Ipv4 address
*/
uint32_t tw_get_ip_addr(char *portName);

/** Return the number of TSC cycles since boot
* @return
*   the number of cycles 
*/
inline uint64_t tw_get_tsc_hz(void);

/** sleep for specified number of milli-seconds 
*/
inline void tw_sleep_msec(unsigned);

/** sleep for specified number of micro-seconds 
*/
inline void tw_sleep_usec(unsigned);

/** sleep for specified number of nano-seconds 
*/
inline void tw_sleep_nsec(unsigned);

int tw_eth_port_init(void); /**< initialize and configure the all available ethernet ports */
uint8_t tw_convert_str_to_iparr(const char *, uint8_t *);
void tw_ip_dec_to_arr( uint32_t ip, uint8_t *arr);
uint64_t tw_convert_str_to_int(const char *str, uint8_t n);
uint32_t tw_ip_arr_to_dec(uint8_t *ip);/**< convert ipaddress array [1,1,1,1] to decimal i.e, 16843009*/
uint32_t tw_convert_str_to_hex(char * str, uint8_t length);/**< convert string with given length to hexadecimal */
extern cJSON * tw_parse_json_file(const char *); /**< parse the JSON file */
extern int Printing_Enable;


#endif

