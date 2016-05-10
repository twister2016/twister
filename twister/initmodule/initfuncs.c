#ifndef _INITFUNCS_C_
#define _INITFUNCS_C_
#include <unistd.h>
#include <sys/types.h>
#include <initfuncs.h>

int argc1=0;
char * argv1[]={NULL};
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

int tw_parse_conf(const char * tw_conf_path){

    cJSON * json_file = NULL;
    json_file = tw_parse_json_file(tw_conf_path);
    int i =0;
    int PCI_ID_LEN = 12;  // "0000:00:00.0"

    char * portmask = cJSON_GetObjectItem(json_file, "portmask")->valuestring;
    char * coremask = cJSON_GetObjectItem(json_file, "coremask")->valuestring;

    cJSON * blacklist_object = cJSON_GetObjectItem(json_file, "blacklist");
    int blacklist_size = cJSON_GetArraySize(blacklist_object);

    char ** blacklist_array = malloc(blacklist_size * sizeof(char*));
    for (i=0; i<blacklist_size; i++) {
        blacklist_array[i] = malloc(PCI_ID_LEN * sizeof(char*) );
    }
    for (i =0; i< blacklist_size; i++) {
        blacklist_array[i] = cJSON_GetArrayItem(blacklist_object, i)->valuestring;
    }


    twister_config.portmask = portmask;
    twister_config.coremask = coremask;
    twister_config.blacklist = blacklist_array; // pci IDs of blacklisted devices
    twister_config.blacklist_size = blacklist_size; //size of blacklist

    return 0;
}

int tw_init_eal_env(int argc, char **argv) {
	if (getuid())
	{
	 printf("%s", "Permission denied: Please run the application using sudo to use Hugepages!\n");
	 exit(0);
	}
    rte_set_log_level(RTE_LOG_ERR);
    rte_set_log_type(RTE_LOGTYPE_EAL,0);
    rte_set_log_type(RTE_LOGTYPE_PMD,0); 
    tw_parse_conf("/home/twister/config/twister_api.json");
    argv[0] = (char *)malloc(3 * sizeof(char));
    argv[1] = (char *)malloc(3 * sizeof(char));
    argv[2] = (char *)malloc(5 * sizeof(char));
    argv[3] = (char *)malloc(3 * sizeof(char));
    argv[4] = (char *)malloc(2 * sizeof(char));
    argv[5] = (char *)malloc(3 * sizeof(char));
    argv[6] = (char *)malloc(12 * sizeof(char));
    argv[7] = (char *)malloc(3 * sizeof(char));
    argv[8] = (char *)malloc(3 * sizeof(char));
    argv[9] = (char *)malloc(4 * sizeof(char));
    strcpy (argv[0], "ab");
    strcpy (argv[1], "-c");
    strcpy (argv[2], twister_config.coremask);
    strcpy (argv[3], "-n");
    strcpy (argv[4], "4");
    strcpy (argv[5], "-b");
    strcpy (argv[6], twister_config.blacklist[0]);   // TODO update argv[6] to store all PCI ids
    strcpy (argv[7], "--");
    strcpy (argv[8], "-p");
    strcpy (argv[9], twister_config.portmask);
    argc = 10;
    
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


int tw_init_global(void) {
        tw_init_eal_env(argc1, argv1);
	tw_init_user_given_vals();
	tw_create_rx_tx_mempools();
	tw_lcore_conf_init();
	tw_eth_port_init();	
	tw_init_timer_vals();
	tw_init_periodic_timers();
	
	return 0;
}
int tw_init_user_given_vals(void) {
	tw_get_port_conf_json_vals("/home/twister/config/config.json");
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
