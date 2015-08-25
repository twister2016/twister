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

	/*if(PIPELINE==1)
	{
	uint32_t lcores[3], n_lcores, lcore_id;
	n_lcores = 0;
	for (lcore_id = 0; lcore_id < RTE_MAX_LCORE; lcore_id++) {
		if (rte_lcore_is_enabled(lcore_id) == 0)
			continue;

		if (n_lcores >= 3) {
			RTE_LOG(ERR, USER1, "Number of cores must be 3\n");
			return -1;
		}

		lcores[n_lcores] = lcore_id;
		n_lcores++;
	}

	if (n_lcores != 3) {
		RTE_LOG(ERR, USER1, "Number of cores must be 3\n");
		return -1;
	}

	app.core_rx = lcores[0];
	app.core_worker = lcores[1];
	app.core_tx = lcores[2];
    }
	*/
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

static void
app_init_rings(void)
{
	uint32_t i;

	for (i = 0; i < app.n_ports; i++) {
		char name[32];

		snprintf(name, sizeof(name), "app_ring_rx_%u", i);

		app.rings_rx[i] = rte_ring_create(
			name,
			app.ring_rx_size,
			rte_socket_id(),
			RING_F_SP_ENQ | RING_F_SC_DEQ);

		if (app.rings_rx[i] == NULL)
			rte_panic("Cannot create RX ring %u\n", i);
	}

	for (i = 0; i < app.n_ports; i++) {
		char name[32];

		snprintf(name, sizeof(name), "app_ring_tx_%u", i);

		app.rings_tx[i] = rte_ring_create(
			name,
			app.ring_tx_size,
			rte_socket_id(),
			RING_F_SP_ENQ | RING_F_SC_DEQ);

		if (app.rings_tx[i] == NULL)
			rte_panic("Cannot create TX ring %u\n", i);
	}

}

int tw_init_global(int argc, char **argv) {
	printf("tw_init_global1\n");
	init_user_given_vals();
	init_eal_env(argc, argv);
	printf("tw_init_global1\n");
	create_rx_tx_mempools();
	printf("tw_init_global1\n");
	create_queued_pkts_mempools();
	if(PIPELINE==0)
	{
		lcore_conf_init();
	}
	if(PIPELINE==1)
	{
		lcore_pipeline_init();
		app_init_rings();
	}
	eth_port_init();
	
	init_timer_vals();
	init_periodic_timers();
	
	
	return 0;
}
int init_user_given_vals(void) {
	get_port_conf_json_vals("port_conf");
	printf("init_user_given_vals1\n");
	get_lcore_queue_conf_json_vals("lcore_queue_conf");
	printf("init_user_given_vals1\n");
	get_processing_conf_json_vals("processing_conf");
	printf("init_user_given_vals1\n");
	queued_pkt_time_limit = 10;			//--!TODO use file values parsed by jSON
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
