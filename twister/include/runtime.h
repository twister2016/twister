#ifndef _RUNTIME_H_
#define _RUNTIME_H_

extern void tw_app_main_loop_rx(void);
extern void tw_app_main_loop_tx(void);
extern void tw_app_main_loop_worker(void);
extern void tw_add_packet_to_tx_pipeline(struct rte_mbuf *, uint8_t);
#endif
