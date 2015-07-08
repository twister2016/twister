#include <stdio.h>
#include <stdint.h>
#include <rx.h>
#include <initfuncs.h>

//int PIPELINE=0; 

int main (int, char **);
int launch_one_lcore(__attribute__((unused)) void *);

int main(int argc, char **argv ) {
	init_global(argc, argv);
	rte_eal_mp_remote_launch(launch_one_lcore, NULL, CALL_MASTER);
	return 0;	
}

int launch_one_lcore(__attribute__((unused)) void *dummy) {
	unsigned i, j, nb_rx;
	int sockfd = udp_socket(port_info[0].start_ip_addr, 8787);
	
	
        return 0;
}
