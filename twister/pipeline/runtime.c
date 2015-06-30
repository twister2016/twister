/*-
 *   BSD LICENSE
 *
 *   Copyright(c) 2010-2014 Intel Corporation. All rights reserved.
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of Intel Corporation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <string.h>
#include <sys/queue.h>
#include <stdarg.h>
#include <errno.h>
#include <getopt.h>


#include <initfuncs.h>
#include <runtime.h>

void
app_main_loop_rx(void) {
	uint32_t i;
	int ret;
	int j=0;
	struct rte_mbuf *m;

	RTE_LOG(INFO, USER1, "Core %u is doing RX\n", rte_lcore_id());

	for (i = 0; ; i = ((i + 1) & (app.n_ports - 1))) {
		uint16_t n_mbufs;

		n_mbufs = rte_eth_rx_burst(
			app.ports[i],
			0,
			app.mbuf_rx.m_table,
			app.burst_size_rx_read);

		if (n_mbufs == 0)
			continue;
		// Remove this for loop
		for (j = 0; j < n_mbufs; j++) {
				m = app.mbuf_rx.m_table[j];
				rte_prefetch0(rte_pktmbuf_mtod(m, void *));
				rte_pktmbuf_dump(stdout,m,100);
			}

		do {
			ret = rte_ring_sp_enqueue_bulk(
				app.rings_rx[i],
				(void **) app.mbuf_rx.m_table,
				n_mbufs);
		} while (ret < 0);
	}
}


void
app_main_loop_worker(void) {
	struct mbuf_table *worker_mbuf;
	uint32_t i;

	RTE_LOG(INFO, USER1, "Core %u is doing work (no pipeline)\n",
		rte_lcore_id());

	worker_mbuf = rte_malloc_socket(NULL, sizeof(struct mbuf_table),
			RTE_CACHE_LINE_SIZE, rte_socket_id());
	if (worker_mbuf == NULL)
		rte_panic("Worker thread: cannot allocate buffer space\n");

	for (i = 0; ; i = ((i + 1) & (app.n_ports - 1))) {
		int ret;

		ret = rte_ring_sc_dequeue_bulk(
			app.rings_rx[i],
			(void **) worker_mbuf->m_table,
			app.burst_size_worker_read);

		if (ret == -ENOENT)
			continue;

		do {
			ret = rte_ring_sp_enqueue_bulk(
				app.rings_tx[i ^ 1],
				(void **) worker_mbuf->m_table,
				app.burst_size_worker_write);
		} while (ret < 0);
	}
}

void
app_main_loop_tx(void) {
	uint32_t i;

	RTE_LOG(INFO, USER1, "Core %u is doing TX\n", rte_lcore_id());

	for (i = 0; ; i = ((i + 1) & (app.n_ports - 1))) {
		uint16_t len, n_pkts;
		int ret;

		len = app.mbuf_tx[i].len;

		ret = rte_ring_sc_dequeue_bulk(
			app.rings_tx[i],
			(void **) &app.mbuf_tx[i].m_table[len],
			app.burst_size_tx_read);

		if (ret == -ENOENT)
			continue;

		len += app.burst_size_tx_read;

		if (len < app.burst_size_tx_write) {
			app.mbuf_tx[i].len = len;
			continue;
		}

		n_pkts = rte_eth_tx_burst(
			app.ports[i],
			0,
			app.mbuf_tx[i].m_table,
			len);

		if (n_pkts < len) {
			uint16_t k;

			for (k = n_pkts; k < len; k++) {
				struct rte_mbuf *pkt_to_free;

				pkt_to_free = app.mbuf_tx[i].m_table[k];
				rte_pktmbuf_free(pkt_to_free);
			}
		}

		app.mbuf_tx[i].len = 0;
	}
	
}
