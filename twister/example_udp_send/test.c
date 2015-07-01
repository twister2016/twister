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
	int sockfd = udp_socket(port_info[0].start_ip_addr,7898);
	struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
	struct rte_mbuf *m;
	uint8_t port_id = 0;
	char data[]={'t','e','s','t','1'};
	
	while (1) {
			sleep(1);
			printf("\n\n***************\n");
			print_arp_table();
        		udp_send(sockfd,data,5,convert_ip_str_to_dec("11.11.7.166"),8787);
			twister_timely_burst();
			nb_rx = rte_eth_rx_burst(port_id, 0, pkts_burst, 16);
			for (j = 0; j < nb_rx; j++) {
				m = pkts_burst[j];
				rte_prefetch0(rte_pktmbuf_mtod(m, void *));
				eth_pkt_parser(m, port_id);
			}
	}

        return 0;
}
