#ifndef _INITFUNCS_C_
#define _INITFUNCS_C_

#include <initfuncs.h>

struct app_params app = {
	/* Ports*/
	.n_ports = MAX_ETH_PORTS,
	.port_rx_ring_size = 128,
	.port_tx_ring_size = 512,

	/* Rings */
	.ring_rx_size = 128,
	.ring_tx_size = 128,

	/* Buffer pool */
	.pool_buffer_size = 2048 + sizeof(struct rte_mbuf) +
		RTE_PKTMBUF_HEADROOM,
	.pool_size = 32 * 1024,
	.pool_cache_size = 256,

	/* Burst sizes */
	.burst_size_rx_read = 32,
	.burst_size_rx_write = 32,
	.burst_size_worker_read = 32,
	.burst_size_worker_write = 32,
	.burst_size_tx_read = 32,
	.burst_size_tx_write = 32,
};

int tw_init_eal_env(int argc, char **argv) {

    if ( argc == 1 ) {
    const char *temp0, *temp1, *temp2, *temp3, *temp4, *temp5, *temp6, *temp7, *temp8;
    temp0 = "-c";
    temp1 = "0x01";
    temp2 = "-n";
    temp3 = "1";
    temp4 = "-b";
    temp5 = "00:03.0";
    temp6 = "--";
    temp7 = "-p";
    temp8 = "0x1";
    //argv = (char **)malloc(argc * sizeof(char *));
    argv[1] = (char *)malloc(3 * sizeof(char));
    argv[2] = (char *)malloc(5 * sizeof(char));
    argv[3] = (char *)malloc(3 * sizeof(char));
    argv[4] = (char *)malloc(2 * sizeof(char));
    argv[5] = (char *)malloc(3 * sizeof(char));
    argv[6] = (char *)malloc(8 * sizeof(char));
    argv[7] = (char *)malloc(3 * sizeof(char));
    argv[8] = (char *)malloc(3 * sizeof(char));
    argv[9] = (char *)malloc(4 * sizeof(char));
    strcpy (argv[1], temp0);
    strcpy (argv[2], temp1);
    strcpy (argv[3], temp2);
    strcpy (argv[4], temp3);
    strcpy (argv[5], temp4);
    strcpy (argv[6], temp5);
    strcpy (argv[7], temp6);
    strcpy (argv[8], temp7);
    strcpy (argv[9], temp8);
    argc = 10;
    }
    
    	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid EAL arguments\n");
	argc -= ret;
	argv += ret;
	ret = tw_parse_twister_args(argc, argv);     
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Invalid commandline arguments\n");
	return 0;
}

int tw_parse_twister_args(int argc, char **argv) {
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
			app_port_mask = tw_parse_portmask(optarg);
			if (app_port_mask == 0) {
				printf("invalid portmask\n");
				tw_display_usage(prgname);
				return -1;
			}
			break;

		/* long options */
		case 0:
			tw_display_usage(prgname);
			return -1;

		default:
			tw_display_usage(prgname);
			return -1;
		}
	}

	if (optind >= 0)
		argv[optind-1] = prgname;

	ret = optind-1;
	optind = 0; /* reset getopt lib */
	return ret;
}


int tw_init_global(int argc, char **argv) {
	tw_init_eal_env(argc, argv);
	tw_init_user_given_vals();
	tw_create_rx_tx_mempools();
//	tw_create_queued_pkts_mempools();
	tw_lcore_conf_init();
	tw_eth_port_init();	
	tw_init_timer_vals();
	tw_init_periodic_timers();
	
	return 0;
}
int tw_init_user_given_vals(void) {
	tw_get_port_conf_json_vals("port_conf");
//	get_lcore_queue_conf_json_vals("lcore_queue_conf");
	queued_pkt_time_limit = 10;			//--!TODO use file values parsed by jSON
	return 0;				//--!JSON...port ips...num of rx/tx queues...flags...vlan tag
}

int tw_display_usage(const char * prgname) {
	printf("%s bad argument\n", prgname);
	return 0;
}

int tw_parse_portmask(const char *portmask)
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
