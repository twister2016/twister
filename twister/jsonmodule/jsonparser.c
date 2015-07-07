#ifndef _JSONPARSER_C_
#define _JSONPARSER_C_

#include <jsonparser.h>
#include <initfuncs.h>

/* Read a file, parse, render back, etc. */
char * registerfile (const char *filename) {
	FILE *f;long len;char *data;
	
	f=fopen(filename,"rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);
	if(!fread(data,1,len,f))
		printf("No data read from file %s\n", filename);
	fclose(f);
	return data;
}

cJSON * parse_json_file(const char * file_name) {
	char * rawjson = registerfile(file_name); 
	cJSON * json_config_file;

	json_config_file=cJSON_Parse(rawjson);
	return json_config_file;
}

int get_port_conf_json_vals(const char * file_name) {
	uint8_t i, j;
	cJSON * json_file = parse_json_file(file_name);
	if (!json_file) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}
	for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {			//For each port_num

		cJSON * subitem = cJSON_GetArrayItem(json_file, i);
		uint8_t port_num = convert_str_to_int(cJSON_GetObjectItem(subitem, "port_num")->valuestring, 2);	//TODO Remove "2" and use sizeof
		cJSON * ip_addrs = cJSON_GetObjectItem(subitem, "ip_addrs");

		for (j = 0 ; j < cJSON_GetArraySize(ip_addrs) ; j++) {			//For each start_ip_addr and range
	        	cJSON * subdictip = cJSON_GetArrayItem(ip_addrs, j);
			port_info[port_num].start_ip_addr = convert_ip_str_to_dec(cJSON_GetObjectItem(subdictip, "start_ip_addr")->valuestring);
        		port_info[port_num].num_ip_addrs = convert_str_to_int(cJSON_GetObjectItem(subdictip, "num_ip_addrs")->valuestring, 2);	//TODO add string lenght logic
    		}
		port_info[port_num].gateway_ip = convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "gateway_ip")->valuestring);
		port_info[port_num].subnet_mask = convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "subnet_mask")->valuestring);
		port_info[port_num].vlan_tag = convert_str_to_int(cJSON_GetObjectItem(subitem, "vlan_tag")->valuestring, 4);

		port_info[port_num].flags = convert_str_to_hex(cJSON_GetObjectItem(subitem, "flags")->valuestring, sizeof(uint64_t));
		port_info[port_num].num_rx_queues = convert_str_to_int(cJSON_GetObjectItem(subitem, "num_rx_queues")->valuestring, 2);
		port_info[port_num].num_tx_queues = convert_str_to_int(cJSON_GetObjectItem(subitem, "num_tx_queues")->valuestring, 2);
	}
	return 0;
}

int get_lcore_queue_conf_json_vals(const char * file_name) {
	uint8_t i, j;
	cJSON * json_file = parse_json_file(file_name);
	if (!json_file) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}

	for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {	
		cJSON * subitem = cJSON_GetArrayItem(json_file, i);
		uint8_t lcore_id = convert_str_to_int(cJSON_GetObjectItem(subitem, "lcore_id")->valuestring, 2);
		lcore_conf[lcore_id].core_rx = convert_str_to_int(cJSON_GetObjectItem(subitem, "core_rx")->valuestring, 1);
		lcore_conf[lcore_id].core_tx = convert_str_to_int(cJSON_GetObjectItem(subitem, "core_tx")->valuestring, 1);
		lcore_conf[lcore_id].core_working = convert_str_to_int(cJSON_GetObjectItem(subitem, "core_working")->valuestring, 1);
		cJSON * ports = cJSON_GetObjectItem(subitem, "ports");

		for(j = 0; j < cJSON_GetArraySize(ports); j++) {
			cJSON * per_queue = cJSON_GetArrayItem(ports, j);
			lcore_conf[lcore_id].mngd_queues[lcore_conf[lcore_id].num_queues].port_id = convert_str_to_int(cJSON_GetObjectItem(per_queue, "port_id")->valuestring, 2);
			lcore_conf[lcore_id].mngd_queues[lcore_conf[lcore_id].num_queues].queue_id = convert_str_to_int(cJSON_GetObjectItem(per_queue, "queue_id")->valuestring, 2);
			lcore_conf[lcore_id].num_queues++;
		}
	}
	return 0;
}

int get_processing_conf_json_vals(const char * file_name) {
	uint8_t i;
	cJSON * json_file = parse_json_file(file_name);
	if (!json_file) {
		printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		return -1;
	}
	for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {
		cJSON * subitem = cJSON_GetArrayItem(json_file, i);
		PIPELINE = convert_str_to_int(cJSON_GetObjectItem(subitem, "pipeline_processing")->valuestring, 2);
		
	}
	return 0;
}

#endif
