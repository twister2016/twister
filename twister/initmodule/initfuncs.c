#ifndef _INITFUNCS_C_
#define _INITFUNCS_C_

#include <initfuncs.h>

int init_eal_env(int argc, char **argv) {
	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid EAL arguments\n");
	argc -= ret;
	argv += ret;
	ret = parse_twister_args(argc, argv);	//--! TODO implement parse_twister_args()
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid commandline arguments\n");
	return 0;
}

int parse_twister_args(int argc, char **argv) {
	int opt, ret;
	char **argvopt;
	int option_index;
	char *prgname = argv[0];
	static struct option lgopts[] = {
		{NULL, 0, 0, 0}
	};

	argvopt = argv;

	while ((opt = getopt_long(argc, argvopt, "p:",
				  lgopts, &option_index)) != EOF) {

		switch (opt) {
		/* portmask */
		case 'p':
			app_port_mask = parse_portmask(optarg);
			if (app_port_mask == 0) {
				printf("invalid portmask\n");
				display_usage(prgname);
				return -1;
			}
			break;

		/* long options */
		case 0:
			display_usage(prgname);
			return -1;

		default:
			display_usage(prgname);
			return -1;
		}
	}

	if (optind >= 0)
		argv[optind-1] = prgname;

	ret = optind-1;
	optind = 0; /* reset getopt lib */
	return ret;
}

int init_global(int argc, char **argv) {
	init_eal_env(argc, argv);
	lcore_conf_init();
	create_rx_tx_mempools();
	create_queued_pkts_mempools();
	init_user_given_vals();
	eth_port_init();
	init_timer_vals();
	init_periodic_timers();
	return 0;
}
int init_user_given_vals(void) {
	get_port_conf_json_vals("port_conf");
	get_lcore_queue_conf_json_vals("lcore_queue_conf");
	queue_time_limit = 15;			//--!TODO use file values parsed by jSON
	return 0;				//--!JSON...port ips...num of rx/tx queues...flags...vlan tag
}

int display_usage(const char * prgname) {
	printf("%s bad argument\n", prgname);
	return 0;
}

int parse_portmask(const char *portmask)
{
	char *end = NULL;
	unsigned long pm;

	/* parse hexadecimal string */
	pm = strtoul(portmask, &end, 16);
	if ((portmask[0] == '\0') || (end == NULL) || (*end != '\0'))
		return -1;

	if (pm == 0)
		return -1;

	return pm;
}

#endif
