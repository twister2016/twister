
#include <initfuncs.h>
#include <lcoreconf.h>

void assign_ports_to_cores(void);

int engine_name_to_lcore_id(char* engineName) {
    int i;
    int engn_id = -1;
    for (i = 0; i < MAX_LCORES; i++) {
        if (lcore_conf[i].core_name != NULL) {
            if (strcmp(lcore_conf[i].core_name, engineName) == 0)
                engn_id = i;
        } else {
            printf("NULL VALUE FROM JSON engine_name_to_lcore....................\n");
        }

    }
    return engn_id;
}


int lcore_conf_init(void) {
    uint8_t lcore_id = 0;
    uint8_t lcore_info_counter = 0;
    char engine_string[10];

    for (lcore_id = 0; lcore_id < MAX_LCORES; lcore_id++) {

        if (rte_lcore_is_enabled(lcore_id)) {
            lcore_conf[lcore_id].core_id_external = lcore_info_counter;
            sprintf(engine_string, "engine%d", lcore_info_counter);
            strcpy(lcore_conf[lcore_id].core_name, engine_string);
            lcore_info_counter++;
            lcore_conf[lcore_id].socket_id = rte_lcore_to_socket_id(lcore_id);
            uint8_t current_lcore_cpuid = rte_lcore_to_socket_id(lcore_id);
            if ( (current_lcore_cpuid+1) >= num_numa_sockets)
                num_numa_sockets = current_lcore_cpuid; //--!Considering there are 2 CPU sockets: Socket 0 and 1

            lcore_conf[lcore_id].core_rx = 1;
            lcore_conf[lcore_id].core_tx = 1;
            lcore_conf[lcore_id].core_working = 1;
            lcore_conf[lcore_id].num_port = 0;

            //--!TODO implement assign_queue_to_core
        }
    }
    return 0;
}

int tw_map_port_to_engine(char portName[10], char coreName[10]) {
    
    int port_id_resolved = eth_name_to_id(portName);
    int engine_id_resolved = engine_name_to_lcore_id(coreName);
//    printf("engine_id_resolved = %d  , port_id_resolved=%d\n", port_id_resolved,engine_id_resolved);
    if (port_id_resolved != -1 && engine_id_resolved != -1) { 
        lcore_conf[engine_id_resolved].managed_port[lcore_conf[engine_id_resolved].num_port] = port_id_resolved;
        lcore_conf[engine_id_resolved].num_port++;

    } else {
        printf("there is problem in tw_map_port_to_engine000000000000000\n");
        printf("port_id_resolved = %d portName=%s\n", port_id_resolved, portName);
        printf("engine_id_resolved = %d  engineName=%s\n", engine_id_resolved, coreName);
    }
    
    return 0;


}

