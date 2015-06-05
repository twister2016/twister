#ifndef _INITFUNCS_C_
#define _INITFUNCS_C_

#include <initfuncs.h>

int init_eal_env(int argc, char **argv) {
	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid EAL arguments\n");
	argc -= ret;
	argv += ret;
	parse_twister_args();			//--! TODO implement parse_twister_args()
	return 0;
}

int parse_twister_args(void) {
	return 0;
}

int init_global(int argc, char **argv) {
	init_eal_env(argc, argv);
	create_rx_tx_mempools();
	create_queued_pkts_mempools();
	lcore_conf_init();
	eth_port_init();
	init_timer_vals();
	init_periodic_timers();
	init_user_given_vals();
	return 0;
}
int init_user_given_vals(void) {
	get_port_json_values("port_conf_file");
	queue_time_limit = 15;			//--!TODO use file values parsed by jSON
	return 0;				//--!JSON...port ips...num of rx/tx queues...flags...vlan tag
}

#endif
