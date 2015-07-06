#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_

#include <rte_lcore.h>

struct event_io {
	int sock_fd;
	void * event_cb;	//TODO args for cb
	void * event_cb_args;
	uint8_t event_repeat;
	struct event_io * next;
};

struct event_io * root_event_io[MAX_LCORES];

struct event_io * 
init_io_event(int sock_fd, void * cb_func, void * cb_args, uint8_t repeat_event) {
	int core_id = rte_lcore_id();   //TODO use proper lcore id
	struct event_io * temp_event_io = root_event_io[core_id];
	if(root_event_io[core_id] == NULL) {
		root_event_io[core_id] = rte_malloc("struct event_io *", sizeof(struct event_io), 64);
		root_event_io[core_id]->event_repeat = repeat_event;
		root_event_io[core_id]->sock_fd = sock_fd;
		root_event_io[core_id]->event_cb = cb_func;
		root_event_io[core_id]->event_cb_args = cb_args;
		root_event_io[core_id]->next = NULL;
		return root_event_io[core_id];
	}
	while(temp_event_io->next != NULL)
		temp_event_io = temp_event_io->next;
	temp_event_io = rte_malloc("struct event_io *", sizeof(struct event_io), 64);
        temp_event_io->sock_fd = sock_fd;
        temp_event_io->event_cb = cb_func;
        temp_event_io->event_cb_args = cb_args;
        temp_event_io->next = NULL;
	return temp_event_io;
}

int start_io_event(struct event_io * event_to_start) {
	do {
		
	} while(event_to_start->event_repeat);
}

#endif
