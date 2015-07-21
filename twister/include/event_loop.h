#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_

#include <rte_lcore.h>
#include <simple_queue.h>

#define MAX_LCORES 16
#define INFINITE_LOOP 0

uint8_t event_io_global;
uint8_t event_flags_global;

enum {
	NO_FLAG_SET = 0x0,
	GET_L2_PKTS = 0x1,
	GET_L3_PKTS = 0x2,
	GET_L4_PKTS = 0x4,
	ENABLE_EVENT_RX = 0x8,
	ENABLE_EVENT_TX = 0x10,
	NO_REPEAT_EVENT = 0x0,
	REPEAT_EVENT = 0x1,
	RX_CALLBACK = 0x1,
	TX_CALLBACK = 0x2
};

struct event_io {
	uint8_t type;
        uint8_t lock;
        uint8_t repeat;
	uint8_t flags;
        int sock_fd;
        void * event_cb;        //TODO args for cb
        struct event_io * next;
};

int reg_flag_event(uint8_t, void *);

struct event_io * root_event_io[MAX_LCORES];

struct event_io * reg_io_event(int sock_fd, void * cb_func, uint8_t repeat_event ,uint8_t event_flags, uint8_t event_type);

int start_io_events(uint32_t);
#endif
