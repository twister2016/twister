#ifndef _TWAPI_H_
#define _TWAPI_H_
#include <event_loop.h>
#include <lcoreconf.h>
#include <stats.h>
/**
 * @file
 *
 * Main twister API functions
 */

struct icmp_echo{
	unsigned char type;
	unsigned char code;
	unsigned short checksum;
	unsigned short identifier;
	unsigned short sequence;
};

struct arp_table {
	uint32_t ip_addr;
	struct ether_addr eth_mac;
	uint8_t port_id;
	struct arp_table * next;
} __attribute__((__packed__));

/** Global Initialization function - initialize the user given arguments for twister application
*
* @param argc
*   argument count 
* @param argv
*   argument vector passed by user
* @return
*   TRUE (value 0) else cause panic
*/
int tw_init_global(void);

/** run any given function on availbale engines described in launch_type
*
* @param func
*   function name 
* @param arg
*   argument vector of function
* @param engine_name
*   An engine name in string on which the func(args) would be executed i.e., "engine0", "engine1", "engine2", "engine3".
*/
void tw_launch_engine(void * func, void * arg, char* engine_name);
/** map port name tw0 to engine name engine0, engine1 etc. All the tx,rx processing of that port would be handled by the mapped engine
*
* @param portName
*   tw0, tw1...etc
* @param coreName
*   engine0, engine1.... etc
* @return
*   TRUE(1), False(-1)
 */
int tw_map_port_to_engine(char portName[10], char coreName[10]);
/** unregister a tx event with callback from program event loop
 *
 * @param transmit_handle
 *    tw_tx_s struct which is to be unregisterd from event loop
 * @param ev_loop
 *    A Pointer to an event loop struct
 * @return
 *   TRUE(1), False (-1) 
 */  
uint8_t tw_tx_unregister (tw_tx_t * transmit_handle, tw_loop_t * ev_loop );

/** unregister a timer event with callback from program event loop
 *
 * @param timer_handle
 *    tw_timer_s struct which is to be unregisterd from event loop
 * @param ev_loop
 *    A Pointer to an event loop struct
 * @return
 *   TRUE(1), False (-1) 
 */  
uint8_t tw_timer_unregister (tw_timer_t * timer_handle, tw_loop_t * ev_loop );
/** Initialize an eventloop for a specified number of time
*
* @param time_to_run (seconds)
*   time for which the eventloop should be running. if 0, means infinite loop.
* @return
*   pointer to main event loop struct tw_loop_s
*/
tw_loop_t * tw_default_loop(uint16_t time_to_run);

/** initialize the rx handles for given event loop
*
* @param loop
*   pointer to main event loop struct tw_loop_s.
* @return
*   pointer to struct tw_rx_s that contains informations of callback, flags, last run time for rx event. 
*/
tw_rx_t * tw_rx_init(tw_loop_t * loop);

/** initialize the tx handles for given event loop
*
* @param loop
*   pointer to main event loop struct tw_loop_s
* @return
*   pointer to struct tw_tx_s that contains informations of callback, flags, last run time for tx event 
*/

tw_tx_t * tw_tx_init(tw_loop_t * loop);

/** Register the callback functions for rx handles
*
* @param rx_handle
*   pointer to struct tw_tx_s that contains informations of callback, flags, last run time for rx event 
* @param recv_cb
*   callback functions to be registered for that handle
* @return
*   TRUE (value 0) FALSE(value -1) 
*/
int tw_rx_start(tw_rx_t * rx_handle, void * recv_cb); 

/** Register the callback functions for tx handles
*
* @param tx_handle
*   which is a struct tw_tx_s for informations of callback, flags, last run time for tx event 
* @param tx_cb
*   callback functions to be registered for that handle
* @return
*   TRUE (value 0) FALSE(value -1) 
*/

int tw_tx_start(tw_tx_t * tx_handle, void * tx_cb); 
/** This function runs the main eventloop
*
* @param event_loop
*   pointer to main event loop struct tw_loop_s
* @return
*   TRUE (value 0)
*/ 
int tw_run(tw_loop_t * event_loop);

/** sends a packet from buffer to respective port i.e, tw0, tw1 etc 
*
* @param buffer
*   pointer to struct tw_buf_s, which has memory info
* @param port_name
*   port_name through which packet would be sent, like tw0, tw1...
* @return
*   TRUE (value 0) FALSE (value -1)
*/
inline int tw_send_pkt(tw_buf_t * buffer, char * port_name); 

/** parse arp packets (both requests and replies) for respective port i.e tw0, tw1
*
* @param buffer
*   pointer to struct tw_buf_s, which has memory info 
* @param port_name
*   port_name through which packet has come, like tw0, tw1...
* @return
*   TRUE (value 0) FALSE(value -1) 
*/ 
inline int tw_arp_parser(tw_buf_t * buffer, char * port_name) ;


/** strip the vlan tag from packet 
*
* @param buffer
*   pointer to struct tw_buf_s, which has memory info
*/ 
inline void tw_vlan_strip(tw_buf_t * buffer);

/** search the ip from arp table
*
* @param ip_to_search
*   unsigned32bit integer of IP to search from arp table. 
* @return
*   TRUE (pointer to an element on arp table linklist arp_table ) FALSE(NULL) 
*/  
struct arp_table * tw_search_arp_table(uint32_t ip_to_search); 

/** search the ip from arp table
*
* @param ip_string
*  IPv4 string "x.x.x.x" to search from arp table. 
* @return
*   TRUE (pointer to an element on arp table linklist arp_table ) FALSE(NULL) 
*/
struct ether_addr * tw_search_arp_entry(char *ip_string);
/** create and send the arp request packet for ip through ports ie. tw0, tw1.. where port_id can be find by eth_name_to_id
*
* @param ip
*   unsigned 32-bit integer Ipv4
* @param port_id
*   port_id of ports i.e, tw0, tw1. port_id could be retrieved by eth_name_to_id("tw0")
* @return
*   TRUE (value 0)
*/
int tw_construct_arp_packet(uint32_t ip, uint8_t port_id); 

/** create a memory buffer with given size
*
* @param buffer_size
*   size of memory buffer in integer
* @return
*   tw_buf_t pointer to struct tw_buf_s, which has memory info
*/
tw_buf_t * tw_new_buffer(uint16_t buffer_size); 

/** free a given memory pointed by pointer
*
* @param ptr
*   memory address to free 
*/ 
inline void tw_free(void * ptr); 
/** free a packet from memory buffer, this should be called when you want to free the memory of recevied packet after processing as done in examples
*
* @param buffer
*   packet buffer whose packet memory is to be freed. 
*/ 
inline void tw_free_pkt(tw_buf_t *buffer);
/** copy the buffer from source to destination memory address for given sizes
*
* @param dst
*   destination memory address to which source memory address would be copied
* @param src
*   source memory address
* @param n
*   Number of bytes to copy. 
*/ 
void tw_memcpy (void * dst, const void * src, size_t n); 

/** Create a timer handle/event in the main eventloop
*
* @param loop
*   pointer to main event loop struct tw_loop_s
* @return
*   pointer to tw_timer_t which is tw_timer_s struct that corresponds to timer handler. 
*/
tw_timer_t * tw_timer_init(tw_loop_t * loop); 

/**configuring the timer event 
*
* @param timer_handle
*   pointer to timer handler struct tw_timer_s
* @param timer_cb
*   callback function of type tw_timer_cb, which is executed when timer event occurs.
* @param timeout
*   time in seconds, after which timer event is fired
* @param repeat
*   repeat(1) means timer would be repeated after each time interval, repeat(0) makes it onetime event.
* @return
*   TRUE(0) on success and False(-1) if timer timer_handle is NULL
*/
int tw_timer_start(tw_timer_t* timer_handle, tw_timer_cb timer_cb, uint64_t timeout);

/** create and send the arp request packet for ip through ports ie. tw0, tw1..
*
* @param ip_addr
*   unsigned 32-bit integer Ipv4
* @param port_name
*   name of ports i.e, "tw0", "tw1".
* @return
*   TRUE (value 0)
*/
int tw_send_arp_request(uint32_t ip_addr, char * port_name);

/** Print the ARP table
*
* @return
*   TRUE (value 0)
*/
void tw_print_arp_table(void); 

void *  tw_malloc(char *type, uint32_t size);
#endif
