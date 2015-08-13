#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_

#include <rte_lcore.h>
#include <simple_queue.h>

#define MAX_LCORES 16
#define INFINITE_LOOP 0
#define LOOP_PROCESS 1

///////////////////////////////////////

/*
typedef struct uv_handle_s uv_handle_t;
typedef struct uv_udp_s uv_udp_t;
typedef struct uv_timer_s uv_timer_t;

typedef struct uv_udp_send_s uv_udp_send_t;

typedef void (*uv_udp_send_cb)(uv_udp_send_t* req, int status);
typedef void (*uv_udp_recv_cb)(uv_udp_t* handle,
                               ssize_t nread,
                               const uv_buf_t* buf,
                               const struct sockaddr* addr,
                               unsigned flags);

struct uv_udp_s {
  UV_HANDLE_FIELDS
  size_t send_queue_size;
  size_t send_queue_count;
  UV_UDP_PRIVATE_FIELDS
};

struct uv_udp_send_s {
  UV_REQ_FIELDS
  uv_udp_t* handle;
  uv_udp_send_cb cb;
  UV_UDP_SEND_PRIVATE_FIELDS
};

struct uv_loop_s {
  void* data;
  unsigned int active_handles;
  void* handle_queue[2];
  void* active_reqs[2];
  unsigned int stop_flag;
  UV_LOOP_PRIVATE_FIELDS
};

typedef enum {
  UV_RUN_DEFAULT = 0,
  UV_RUN_ONCE,
  UV_RUN_NOWAIT
} uv_run_mode;

struct uv_handle_s {
  UV_HANDLE_FIELDS
};

struct uv_timer_s {
  UV_HANDLE_FIELDS
  UV_TIMER_PRIVATE_FIELDS
};
*/
///////////////////////////////////////

struct tw_handle_s {
	void * handle_ptr;
	uint64_t last_run_time;
	struct tw_handle_s * next;
	//UV_HANDLE_FIELDS
} __attribute__((__packed__));

struct tw_loop_s {
	/* User data - use this for whatever. */
	void* data;
	/* Loop reference counting. */
	uint8_t active_handles;
	struct tw_handle_s * handle_queue;
	//void* active_reqs[2];
	/* Internal flag to signal loop stop. */
	unsigned int stop_flag;
	uint16_t secs_to_run;
	//UV_LOOP_PRIVATE_FIELDS
} __attribute__((__packed__));

typedef struct tw_loop_s tw_loop_t;
typedef struct tw_handle_s tw_handle_t;

struct event_io {
	uint8_t type;
        uint8_t lock;
        uint8_t repeat;
	uint8_t flags;
        int sock_fd;
        void * event_cb;        //TODO args for cb
        struct event_io * next;
};

struct tw_udp_s {
	void * handle_ptr;
        uint64_t last_run_time;
        struct tw_udp_s * next;
	int sock_fd;
	/* read-only */
	/*
   	* Number of bytes queued for sending. This field strictly shows how much
   	* information is currently queued.
   	*/
  	size_t send_queue_size;
  	/*
   	* Number of send requests currently in the queue awaiting to be processed.
   	*/
  	size_t send_queue_count;
  	//UV_UDP_PRIVATE_FIELDS
};

typedef struct tw_udp_s tw_udp_t;

struct tw_sockaddr_in {   //TODO standardise
	uint32_t sock_ip;
	uint16_t sock_port;
};

int tw_loop_init(tw_loop_t *);
//int tw_loop_close(tw_loop_t *);
int tw_loop_stop(tw_loop_t *);
int tw_loop_run(tw_loop_t *);
tw_loop_t * tw_default_loop(void);
int tw_udp_init(tw_loop_t *, tw_udp_t *);
int tw_udp_bind(tw_udp_t *, struct tw_sockaddr_in *, uint8_t);
struct tw_sockaddr_in * tw_ip4_addr(char *, uint16_t);

#endif
