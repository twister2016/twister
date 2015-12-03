#include <stdio.h>
#include <stdint.h>
#include <tw_api.h>
#include <tw_common.h>
#include <arpa/inet.h>

#define MAX_QUEUE_PACKETS 1000

int main(int, char **);
int user_app_main(void *);
struct ether_addr * dst_eth0;
struct ether_addr * dst_eth1;
struct ether_hdr * eth;
struct ipv4_hdr * ipHdr_d;
uint16_t eth_type;
struct ether_addr * dst_eth_addr;
void reply_payload(tw_rx_t *, tw_buf_t *);

struct route_table {
	uint32_t net_addr;
	uint32_t gateway;
	uint32_t net_mask;
        char  port_name[3];
	struct route_table * next;
} __attribute__((__packed__));

struct sq_pkt {
	tw_buf_t *pkt[MAX_QUEUE_PACKETS];
    	uint32_t n_pkts;
};


struct sq_pkt sq_pkt_q;

struct route_table * route_table_root = NULL;
uint32_t route_table_size = 0;

int tw_sq_push(struct sq_pkt * q_list, tw_buf_t * pkt)
{
    if(q_list->n_pkts >=1000)
        return 0;
    q_list->pkt[q_list->n_pkts] = pkt;
    q_list->n_pkts++;
    return 1;
}

int tw_n_queue(struct sq_pkt* q_list)
{
	if(q_list->n_pkts < 1)
		return 0;
	return q_list->n_pkts;
}

int tw_add_route_entry(uint32_t net_to_add, uint32_t netmask_to_add, uint32_t gateway_to_add, char * port_id) {
    struct route_table * route_table_ptr = route_table_root;
    if (route_table_ptr == NULL) {
        route_table_ptr = tw_malloc("struct route_table", sizeof (struct route_table));
        route_table_root = route_table_ptr;
    } else {
        while (route_table_ptr->next != NULL)
            route_table_ptr = route_table_ptr->next;
        route_table_ptr->next = tw_malloc("struct route_table", sizeof (struct route_table));
        route_table_ptr = route_table_ptr->next;
    }
    if (route_table_ptr == NULL)
        exit(0);

    route_table_ptr->net_addr = tw_be_to_cpu_32(net_to_add);
    route_table_ptr->gateway = tw_be_to_cpu_32(gateway_to_add);
    route_table_ptr->net_mask = tw_be_to_cpu_32(netmask_to_add);
    memcpy(route_table_ptr->port_name ,port_id ,3 );
    
    route_table_ptr->next = NULL;
    route_table_size++;
    return 0;
}

void tw_print_route_table(void) {
    printf("\nTwister Routing table\n");
    printf("\n|------Network----||---Destination---||-----NetMask----||----iface----|\n");
    struct in_addr ip_addr;
    struct route_table * temp_route_entry = route_table_root;
    while (temp_route_entry != NULL) {
        ip_addr.s_addr = temp_route_entry->net_addr;
        printf("|    %s ", inet_ntoa(ip_addr));
        ip_addr.s_addr = temp_route_entry->gateway;
        printf("|    %s ", inet_ntoa(ip_addr));
        ip_addr.s_addr = temp_route_entry->net_mask;
        printf("|    %s ", inet_ntoa(ip_addr));
        printf("|    %s ", temp_route_entry->port_name); 
        temp_route_entry = temp_route_entry->next;
        printf("\n");
    }
    printf("\n");
    return;

}

struct route_table* tw_search_route(uint32_t ip_to_search) {
    struct route_table * temp_route_entry = route_table_root;
    uint32_t temp_ip; 
    while (temp_route_entry != NULL) {
        temp_ip = temp_route_entry ->net_mask & ip_to_search;
        if (temp_ip == temp_route_entry->net_addr)
            return temp_route_entry;
        else
            temp_route_entry = temp_route_entry->next;
    }
    return NULL;

}

int parse_user_params(char * file_name) {
 uint8_t i;
 cJSON * json_file = tw_parse_json_file(file_name); 
 if (!json_file) {
  printf("Error before: [%s]\n",cJSON_GetErrorPtr());
  return -1;
 }
        uint32_t net_addr_tmp;
        uint32_t gateway_tmp;
        uint32_t net_mask_tmp;
        char* port_name_tmp;

 for (i = 0 ; i < cJSON_GetArraySize(json_file) ; i++) {
            cJSON * subitem = cJSON_GetArrayItem(json_file, i);
            net_addr_tmp = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "net_addr")->valuestring);
            gateway_tmp = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "gateway")->valuestring);
            net_mask_tmp = tw_convert_ip_str_to_dec(cJSON_GetObjectItem(subitem, "net_mask")->valuestring);
            port_name_tmp = (cJSON_GetObjectItem(subitem, "port_name")->valuestring);
            tw_add_route_entry(net_addr_tmp,net_mask_tmp,gateway_tmp,port_name_tmp);
 }
 return 0;
}

void reply_payload(tw_rx_t * handle, tw_buf_t * buffer) {
    eth = buffer->data;
    eth_type = tw_be_to_cpu_16(eth->ether_type);
        switch (eth_type) {
            case ETHER_TYPE_ARP:
                tw_arp_parser(buffer, buffer->port_name);
                break;
            case ETHER_TYPE_IPv4:
                ipHdr_d = buffer->data+sizeof(struct ether_hdr);
                struct route_table * temp_route_ptr =tw_search_route(ipHdr_d->dst_addr);
                if(temp_route_ptr == NULL)
                {
                    tw_free_pkt(buffer);
                    return;
                }
                //ipHdr_d->time_to_live--;
                struct arp_table * temp_arp_entry = tw_search_arp_table(ipHdr_d->dst_addr);
                if(temp_arp_entry == NULL) {
                    tw_send_arp_request(ipHdr_d->dst_addr, temp_route_ptr->port_name);
                    tw_sq_push(&sq_pkt_q , buffer);
                    return;
                }
                else {
                dst_eth_addr = &temp_arp_entry->eth_mac;	
                tw_copy_ether_addr(dst_eth_addr, &(eth->d_addr));
                if (strcmp(temp_route_ptr->port_name,"tw0")==0 )
                {
                        tw_copy_ether_addr(dst_eth0, &(eth->s_addr));
                        tw_send_pkt(buffer, "tw0");
                }
                if (strcmp(temp_route_ptr->port_name,"tw1")==0 )
                {
                       tw_copy_ether_addr(dst_eth1, &(eth->s_addr));
                       tw_send_pkt(buffer, "tw1");
                    
                }
                }
                break;       
        }
}

void update_queue()
{
    int i;
    int num=tw_n_queue(&sq_pkt_q);
    if( num <1 )
        return;
    for(i=0;i<1000;i++)
    {
        if(sq_pkt_q.pkt[i]!=NULL)
        {    
            reply_payload(NULL, sq_pkt_q.pkt[i]);
            sq_pkt_q.pkt[i]=NULL;
            sq_pkt_q.n_pkts--;
        }
        
    }
   
        
}

int main(int argc, char **argv) {
     const char *temp0, *temp1, *temp2, *temp3, *temp4, *temp5, *temp6, *temp7, *temp8;
    temp0 = "-c";
    temp1 = "0x0C";
    temp2 = "-n";
    temp3 = "4";
    temp4 = "-b";
    temp5 = "00:03.0";
    temp6 = "--";
    temp7 = "-p";
    temp8 = "0x3";
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
    tw_init_global(argc, argv);
    Printing_Enable = 0;
    tw_map_port_to_engine("tw0", "engine0");
    dst_eth0=tw_get_ether_addr("tw0");
    tw_map_port_to_engine("tw1", "engine0");
    dst_eth1=tw_get_ether_addr("tw1");
    parse_user_params("gw_conf");
    tw_print_route_table();
    int i=0;
    for(i=0;i<1000;i++)
    {
        sq_pkt_q.pkt[i]=NULL;
           
    }
    user_app_main(NULL);   
    return 0;
}

int user_app_main(__attribute__((unused)) void * app_params) {

    tw_rx_t * server;
    tw_timer_t * timer_handle;
    int status;
    tw_loop_t * tw_loop = tw_default_loop(INFINITE_LOOP);

    server = tw_rx_init(tw_loop);
    if (server == NULL) {
        printf("Error in RX init\n");
        exit(1);
    }

    status = tw_rx_start(server, reply_payload);
    if (status) {
        printf("Error in receive start\n");
        exit(1);
    }
    timer_handle = tw_timer_init(tw_loop);
    tw_timer_start(timer_handle, update_queue, 250);
    tw_run(tw_loop);
    return 0;
}
