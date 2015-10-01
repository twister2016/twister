#ifndef _TWAPI_H_
#define _TWAPI_H_
#include <event_loop.h>
/**
 * @file
 *
 * Main twister API functions
 */

extern int tw_init_global(int argc, char** argv); 
extern void tw_launch_engine(void * func, void * arg, int launch_type); 

extern tw_loop_t * tw_default_loop(uint16_t time_to_run);
extern tw_rx_t * tw_rx_init(tw_loop_t *);
extern tw_tx_t * tw_tx_init(tw_loop_t *);
extern int tw_rx_start(tw_rx_t * rx_handle, void * recv_cb); 
extern int tw_run(tw_loop_t *); 
extern int tw_send_pkt(tw_buf_t * buffer, char * port_name); 
extern int tw_arp_parser(tw_buf_t * buffer, char * port_name) ;
extern inline void tw_vlan_strip(tw_buf_t *); 
extern struct arp_table * tw_search_arp_table(uint32_t ip_to_search); 
extern int tw_construct_arp_packet(uint32_t ip, uint8_t port_id); 
extern tw_buf_t * tw_new_buffer(uint16_t buffer_size); 
extern inline void tw_free(void * ptr); 
extern void tw_memcpy (void * dst, const void * src, size_t n); 
extern int tw_tx_start(tw_tx_t *, void * tx_cb); 
extern tw_timer_t * tw_timer_init(tw_loop_t * loop); 
extern int tw_timer_bind(tw_timer_t * timer_handle, struct tw_sockaddr_in * addr, int sock_fd, uint8_t flags);
extern int tw_timer_start(tw_timer_t *, tw_timer_cb, uint64_t, uint64_t);
#endif
