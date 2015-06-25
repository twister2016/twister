#include <stdio.h>
#include <stdint.h>
#include<rx.h>
#include <initfuncs.h>

int main (int, char **);
int launch_one_lcore(__attribute__((unused)) void *);

int main(int argc, char **argv ) {
	init_global(argc, argv);
	int sockfd=udp_socket(185272233,7898);
	char data[]={'s','e','x','y','1'};
	udp_send(sockfd,data,5,185272133,8787);
	
	rte_eal_mp_remote_launch(launch_one_lcore, NULL, CALL_MASTER);
	return 0;	
}

int launch_one_lcore(__attribute__((unused)) void *dummy) {
	unsigned i, j, nb_rx;
	struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
	struct rte_mbuf *m;
	uint8_t portid = 0;
	
	while (1) {
			nb_rx = get_pkt_from_rx_queue(pkts_burst,portid);
			for (j = 0; j < nb_rx; j++) {
				m = pkts_burst[j];
				rte_prefetch0(rte_pktmbuf_mtod(m, void *));
				rte_pktmbuf_dump(NULL,m,100);
				rte_pktmbuf_free(m);			
			}
	}

        return 0;
}
