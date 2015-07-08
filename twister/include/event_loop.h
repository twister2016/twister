#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_

#include <rte_lcore.h>
#define MAX_LCORES 16

enum {
	NO_FLAG_SET = 0x0,
	GET_L2_PKTS = 0x1,
	GET_L3_PKTS = 0x2,
	GET_L4_PKTS = 0x3
};

struct event_io {
        uint8_t event_lock;
        uint8_t event_repeat;
	uint8_t event_flags;
        int sock_fd;
        void * event_cb;        //TODO args for cb
        struct event_io * next;
};

int reg_flag_event(uint8_t, void *);

struct event_io root_event_io[MAX_LCORES];

struct event_io * reg_io_event(int sock_fd, void * cb_func, uint8_t repeat_event);

int start_io_event(struct event_io * event_to_start);
#endif
