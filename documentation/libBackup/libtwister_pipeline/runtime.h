#ifndef _RUNTIME_H_
#define _RUNTIME_H_
/**
 * @file
 *
 * Pipeline Runtime Helpers in Twister
 */
extern void app_main_loop_rx(void);
extern void app_main_loop_tx(void);
extern void app_main_loop_worker(void);
extern void add_packet_to_tx_pipeline(struct rte_mbuf *, uint8_t);
#endif
