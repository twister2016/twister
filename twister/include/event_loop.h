#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_

#include <rte_lcore.h>
#define MAX_LCORES 16

struct event_io {
	int sock_fd;
	void * event_cb;	//TODO args for cb
	void * event_cb_args;
	uint8_t event_repeat;
	struct event_io * next;
};

struct event_loop_flags {
	uint8_t get_l2_packets;
	uint8_t get_l3_packets;
	uint8_t get_l4_packets;	
}; 

struct event_loop_flags event_loop_flags;

struct event_io * root_event_io[MAX_LCORES];

struct event_io * init_io_event(int sock_fd, void * cb_func, void * cb_args, uint8_t repeat_event);

int start_io_event(struct event_io * event_to_start);
#endif
