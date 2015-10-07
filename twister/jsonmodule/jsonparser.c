#ifndef _JSONPARSER_C_
#define _JSONPARSER_C_

#include <jsonparser.h>
#include <initfuncs.h>

/* Read a file, parse, render back, etc. */
char * tw_registerfile (const char *filename) {
	FILE *f;long len;char *data;
	
	f=fopen(filename,"rb");
	if(f == NULL) {
		printf("No file with name %s exists\n", filename);
		rte_exit(EXIT_FAILURE, "No file with specified name found\n");
	}
	fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);
	if(!fread(data,1,len,f))
		printf("No data read from file %s\n", filename);
	fclose(f);
	return data;
}

cJSON * tw_parse_json_file(const char * file_name) {
	char * rawjson = tw_registerfile(file_name); 
	cJSON * json_config_file;

	json_config_file=cJSON_Parse(rawjson);
	return json_config_file;
}

int tw_get_port_conf_json_vals(const char * file_name) {


	uint8_t i, j;
	cJSON * json_file = tw_parse_json_file(file_name);
	if (!json_file) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}
	
	total_eth_ports = rte_eth_dev_count();
	if (total_eth_ports == 0)
		rte_exit(EXIT_FAILURE, "No Ethernet ports\n");
	if (total_eth_ports > MAX_ETH_PORTS)
		total_eth_ports = MAX_ETH_PORTS;
	available_eth_ports = total_eth_ports;
	
	uint8_t port_info_counter=0;
	char eth_string[10];
	uint8_t port_id;
	for (port_id = 0; port_id < total_eth_ports; port_id++) {
		/* skip ports that are not enabled */
		if ((app_port_mask & (1 << port_id)) == 0) {
			available_eth_ports--;
			continue;
		}
		sprintf(eth_string, "tw%d", port_info_counter);
		port_info[port_id].port_id_external= port_info_counter;
		 
        strcpy(port_info[port_id].port_name, eth_string);
		port_info_counter++;
	
    	for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {			//For each port_num

    		cJSON * subitem = cJSON_GetArrayItem(json_file, i);
    		char * curr_port_name_str = cJSON_GetObjectItem(subitem, "port_name")->valuestring;	//TODO Remove "2" and use sizeof 
    		if ( curr_port_name_str!=NULL ) {
    		    if (strcmp(eth_string, curr_port_name_str) != 0)
		            continue; 
    		    		    		
    		    cJSON * ip_addrs = cJSON_GetObjectItem(subitem, "ip_addrs");
    		    port_info[port_id].num_rx_queues = 1;
		        port_info[port_id].num_tx_queues = 1;
    		    for (j = 0 ; j < cJSON_GetArraySize(ip_addrs) ; j++) {			//For each start_ip_addr and range
                	cJSON * subdictip = cJSON_GetArrayItem(ip_addrs, j);
    		    	port_info[port_id].start_ip_addr = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subdictip, "start_ip_addr")->valuestring);
        	    	port_info[port_id].num_ip_addrs = tw_parse_int_from_string( cJSON_GetObjectItem(subdictip, "num_ip_addrs")->valuestring );
    		    	port_info[port_id].gateway_ip = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subdictip, "gateway_ip")->valuestring);
    		        port_info[port_id].subnet_mask = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subdictip, "subnet_mask")->valuestring);	
    		    }
    		}
    		else{printf("NULL VALUE FROM JSON....get_port_conf_json_vals.................\n");}
        		
		}

    }
	return 0;
}

#endif
