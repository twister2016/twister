#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include <rte_config.h>
#include <rte_lcore.h>
#include <rte_ether.h>
#include <mempoolconf.h>
#include <rte_ip.h>
#include <rte_udp.h>
#include "cJSON.h"
//#include <tw_api.h>
#define MAX_LCORES 16
#define INFINITE_LOOP 0
#define LOOP_PROCESS 1

#include <rte_launch.h>
//#include <tw_api.h>
enum {
	USE_ALL_ENGINES = 255,
	ENGINE0 = 0,    //TODO use string in this case
	ENGINE1 = 1,
	ENGINE2 = 2,
	ENGINE3 = 3,
	RESERVE_FIRST_ENGINE = 254
};

//void tw_launch_engine(void *, void *, int);
unsigned tw_current_engine(void);
enum {
    TW_UDP_HANDLE = 0x01,
    TW_TCP_HANDLE = 0x02,
    TW_RX_HANDLE = 0x03,
    TW_TX_HANDLE = 0x04,
    TW_TIMER_HANDLE = 0x05
};

struct tw_handle_s {
    uint8_t handle_type;
    uint64_t last_run_time;
    struct tw_handle_s * next;
    //UV_HANDLE_FIELDS
} __attribute__((__packed__));

struct tw_loop_s {
    /* User data - use this for whatever. */
    void* data;
    /* Loop reference counting. */
    uint8_t active_handles;
    struct tw_rx_s * rx_handle_queue; //TODO expand for TCP also
    struct tw_tx_s * tx_handle_queue;
    struct tw_timer_s * timer_handle_queue;
    //void* active_reqs[2];
    /* Internal flag to signal loop stop. */
    unsigned int stop_flag;
    uint16_t secs_to_run;
    //UV_LOOP_PRIVATE_FIELDS
} __attribute__((__packed__));

typedef struct tw_loop_s tw_loop_t;
typedef struct tw_handle_s tw_handle_t;
//tw_loop_t default_loop[MAX_LCORES];

struct tw_udp_s {
    uint8_t handle_type;
    uint64_t last_run_time;
    int sock_fd;
    struct tw_sockaddr_in * addr;
    uint8_t flags;
    void * alloc_cb;
    void * recv_cb;
    struct tw_udp_s * next;
};

struct tw_rx_s {
    uint8_t handle_type;
    uint64_t last_run_time;
    int sock_fd;
    struct tw_sockaddr_in * addr;
    uint8_t flags;
    void * recv_cb;
    struct tw_rx_s * next;
};

struct tw_tx_s {
    uint8_t handle_type;
    uint64_t last_run_time;
    struct tw_tx_s * next;
    int sock_fd;
    struct tw_sockaddr_in * dst_addr;
    uint8_t flags;
    void * tx_cb;
};

struct tw_timer_s {
    uint8_t handle_type;
    uint64_t last_run_time;
    struct tw_timer_s * next;
    int sock_fd;
    struct tw_sockaddr_in * dst_addr;
    uint8_t flags;
    uint64_t timeout;
    uint64_t repeat;
    void * timer_cb;
};

typedef struct tw_udp_s tw_udp_t;
typedef struct tw_rx_s tw_rx_t;
typedef struct tw_tx_s tw_tx_t;
typedef struct tw_timer_s tw_timer_t;

typedef void * tw_timer_cb;

int tw_loop_init(tw_loop_t *);
//int tw_loop_close(tw_loop_t *);
int tw_stop(tw_loop_t *);
////int tw_run(tw_loop_t *);
//tw_loop_t * tw_default_loop(uint16_t);

////tw_rx_t * tw_rx_init(tw_loop_t *);
////int tw_rx_start(tw_rx_t *, void *);

//tw_tx_t * tw_tx_init(tw_loop_t *);
//int tw_tx_start(tw_tx_t *, void *);

//tw_timer_t * tw_timer_init(tw_loop_t *);
//int tw_timer_bind(tw_timer_t *, struct tw_sockaddr_in *, int, uint8_t);
//int tw_timer_start(tw_timer_t *, tw_timer_cb, uint64_t, uint64_t);

#endif
