#include <stdio.h>
#include <stdint.h>
#include<rx.h>
#include <initfuncs.h>

int main (int, char **);
int app_lcore_main_loop(void *arg);

int main(int argc, char **argv ) {
	uint32_t lcore;
	init_global(argc, argv);
	int sockfd=udp_socket(185272233,7898);
	char data[]={'s','e','x','y','1'};
	//udp_send(sockfd,data,5,185272133,8787);
	

	rte_eal_mp_remote_launch(app_lcore_main_loop, NULL, CALL_MASTER);
	RTE_LCORE_FOREACH_SLAVE(lcore) {
		if (rte_eal_wait_lcore(lcore) < 0)
			return -1;
	}

	return 0;
}

int
app_lcore_main_loop(__attribute__((unused)) void *arg)
{
	unsigned lcore;

	lcore = rte_lcore_id();

	if (lcore == app.core_rx) {
		app_main_loop_rx();
		return 0;
	}
	
		if (lcore == app.core_worker) {
		app_main_loop_worker();
		return 0;
	}

	if (lcore == app.core_tx) {
		app_main_loop_tx();
		return 0;
	}

	return 0;
}
