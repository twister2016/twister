#include <stdio.h>
#include <stdint.h>
#include<rx.h>
#include <initfuncs.h>

int PIPELINE=0; 
int main (int, char **);
int launch_one_lcore(__attribute__((unused)) void *);

int main(int argc, char **argv ) {
	init_global(argc, argv);
	rte_eal_mp_remote_launch(launch_one_lcore, NULL, CALL_MASTER);
	return 0;	
}

int launch_one_lcore(__attribute__((unused)) void *dummy) {
	unsigned i, j, nb_rx;
	struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
	struct rte_mbuf *m;
	uint8_t port_id = 0;
	
	while (1) {
			sleep(1);
			twister_send_burst();
			char data[]={'t','e','s','t','1'};
        		printf("test-1\n");
        		udp_send(sockfd,data,5,convert_ip_str_to_dec(11.11.1.166),8787);
			nb_rx = rte_eth_rx_burst(port_id, 0, pkts_burst, 16);
			for (j = 0; j < nb_rx; j++) {
				m = pkts_burst[j];
				rte_prefetch0(rte_pktmbuf_mtod(m, void *));
				printf("\n\n\n");
				rte_pktmbuf_dump(stdout,m,100);
				printf("\n\n\n");
				rte_pktmbuf_free(m);
			}
	}

        return 0;
}
