#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
/**
 * @file
 *
 * Event Loop Helpers in Twister
 */
#include <rte_lcore.h>
#include <simple_queue.h>

#define MAX_LCORES 16 /**< MAX_LCORES - This macros defines the maximum logical cores. Default value assigned is : 16. */
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

int reg_flag_event(uint8_t, void *); /**< reg_flag_event - This function is used to set the flags in the event loop. For example: One of the flag will be to get all layer 2 packets. Arguments: Flag type and respective callback */

struct event_io * root_event_io[MAX_LCORES];

struct event_io * reg_io_event(int sock_fd, void * cb_func, uint8_t repeat_event ,uint8_t event_flags, uint8_t event_type); /**< reg_io_event - This function registered the event in the event loop. Arguments required by the function are: Socket File Descriptor, Callback to be called on occurrence of the event, and all the registered events are monitored in this function and if any event occurs, Single occurrence Event or Repeated Event and Event type. */

int start_io_events(uint32_t); /**< start_io_events - This function start the event loop and all the registered events are monitored in this function and if any event occurs, its respective callback is called immediately. */
#endif
