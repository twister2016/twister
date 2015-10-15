#ifndef _TWAPI_H_
#define _TWAPI_H_
#include <event_loop.h>
/**
 * @file
 *
 * Main twister API functions
 */
struct arp_table {
	uint32_t ip_addr;
	struct ether_addr eth_mac;
	uint8_t port_id;
	struct arp_table * next;
} __attribute__((__packed__));

int tw_init_global(int argc, char** argv); 
void tw_launch_engine(void * func, void * arg, int launch_type); 

tw_loop_t * tw_default_loop(uint16_t time_to_run);
tw_rx_t * tw_rx_init(tw_loop_t *);
tw_tx_t * tw_tx_init(tw_loop_t *);
int tw_rx_start(tw_rx_t * rx_handle, void * recv_cb); 
int tw_run(tw_loop_t *); 
inline int tw_send_pkt(tw_buf_t * buffer, char * port_name); 
inline int tw_arp_parser(tw_buf_t * buffer, char * port_name) ;
inline void tw_vlan_strip(tw_buf_t *); 
struct arp_table * tw_search_arp_table(uint32_t ip_to_search); 
int tw_construct_arp_packet(uint32_t ip, uint8_t port_id); 
tw_buf_t * tw_new_buffer(uint16_t buffer_size); 
inline void tw_free(void * ptr); 
void tw_memcpy (void * dst, const void * src, size_t n); 
int tw_tx_start(tw_tx_t *, void * tx_cb); 
tw_timer_t * tw_timer_init(tw_loop_t * loop); 
int tw_timer_bind(tw_timer_t * timer_handle, struct tw_sockaddr_in * addr, int sock_fd, uint8_t flags);
int tw_timer_start(tw_timer_t *, tw_timer_cb, uint64_t, uint64_t);
#endif
