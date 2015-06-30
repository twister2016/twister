#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>

int PIPELINE=0; 

int main (int, char **);
int launch_one_lcore(__attribute__((unused)) void *);

int main(int argc, char **argv ) {
	init_global(argc, argv);
	int sockfd=udp_socket(port_info[0].start_ip_addr,7898);
	printf("value sockfd %d\n", sockfd);
	char data[]={'s','e','x','y','1'};
	printf("test-1\n");
	udp_send(sockfd,data,5,185272133,8787);
	printf("rte_eal_mp_remote_launch\n");

	rte_eal_mp_remote_launch(launch_one_lcore, NULL, CALL_MASTER);
	return 0;	
}

int launch_one_lcore(__attribute__((unused)) void *dummy) {
	unsigned i, j, nb_rx;
	struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
	struct rte_mbuf *m;
	uint8_t portid = 0;
	
	while (1) {
			sleep(1);
			printf("num rx pkts %d\n", nb_rx);
			nb_rx = rte_eth_rx_burst(portid, 0, pkts_burst, 16);
			for (j = 0; j < nb_rx; j++) {
				m = pkts_burst[j];
				rte_prefetch0(rte_pktmbuf_mtod(m, void *));
				rte_pktmbuf_dump(stdout,m,100);
				eth_pkt_parser(m, portid);
			}
	}

        return 0;
}
