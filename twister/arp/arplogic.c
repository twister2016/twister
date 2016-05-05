#include <arplogic.h>
#include <initfuncs.h>
#include <arpa/inet.h>
struct arp_table * arp_table_root = NULL;
uint32_t arp_table_size = 0;
struct ether_addr broadcastmac = {
    .addr_bytes[1] = 0xff,
    .addr_bytes[2] = 0xff,
    .addr_bytes[3] = 0xff,
    .addr_bytes[4] = 0xff,
    .addr_bytes[5] = 0xff,
    .addr_bytes[0] = 0xff,
};

struct ether_addr querycastmac = {
    .addr_bytes[1] = 0x00,
    .addr_bytes[2] = 0x00,
    .addr_bytes[3] = 0x00,
    .addr_bytes[4] = 0x00,
    .addr_bytes[5] = 0x00,
    .addr_bytes[0] = 0x00,
};
int info(const char *fname, int lineno) { /* ... */ }
int tw_parse_arp(struct rte_mbuf * pkt, uint8_t port_id) {
    struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
    struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth + 1);
    if (rte_be_to_cpu_16(arp_pkt->arp_op) == ARP_OP_REQUEST) {

        if (rte_be_to_cpu_32(arp_pkt->arp_data.arp_tip) == port_info[port_id].start_ip_addr) { //&& (port_info[port_id].flags & REPLY_ARP)) {
            if (tw_search_arp_table(arp_pkt->arp_data.arp_sip) == NULL) { //Save ARP entry for local use also
                tw_add_arp_entry(arp_pkt->arp_data.arp_sip, arp_pkt->arp_data.arp_sha, port_id);
            }
            tw_send_arp_reply(pkt, port_id);
            return 0;
        }
    } else if (rte_be_to_cpu_16(arp_pkt->arp_op) == ARP_OP_REPLY) {
        tw_process_arp_reply(pkt, port_id);
        return 0;
    }
    rte_pktmbuf_free(pkt);
    return 0;
}

int tw_send_arp_reply(struct rte_mbuf * pkt, uint8_t port_id) {
    struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
    struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth + 1);
    arp_pkt->arp_op = rte_cpu_to_be_16(ARP_OP_REPLY);
    ether_addr_copy(&(arp_pkt->arp_data.arp_sha), &(arp_pkt->arp_data.arp_tha));
    ether_addr_copy(port_info[port_id].eth_mac, &(arp_pkt->arp_data.arp_sha));
    arp_pkt->arp_data.arp_tip = arp_pkt->arp_data.arp_sip;
    arp_pkt->arp_data.arp_sip = rte_cpu_to_be_32(port_info[port_id].start_ip_addr);
    ether_addr_copy(&(eth->s_addr), &(eth->d_addr));
    ether_addr_copy(port_info[port_id].eth_mac, &(eth->s_addr));
    tw_add_pkt_to_tx_queue(pkt, port_id);

    return 0;
}

int tw_process_arp_reply(struct rte_mbuf* pkt, uint8_t port_id) {
    struct ether_hdr * eth = rte_pktmbuf_mtod(pkt, struct ether_hdr *);
    struct arp_hdr * arp_pkt = (struct arp_hdr *) (eth + 1);
    if (tw_search_arp_table(arp_pkt->arp_data.arp_sip) == NULL) {
        //if ARP entry doesn't exist. One MAC can have multiple IPs associated
        if (tw_add_arp_entry(arp_pkt->arp_data.arp_sip, arp_pkt->arp_data.arp_sha, port_id)) {
            rte_pktmbuf_free(pkt);
            return -1;
        }
    }
    rte_pktmbuf_free(pkt);
    return 0;
}

struct arp_table * tw_search_arp_table(uint32_t ip_to_search) {
    ip_to_search = rte_be_to_cpu_32(ip_to_search);
    struct arp_table * arp_table_ptr = arp_table_root;
    while (arp_table_ptr != NULL) {
        if (arp_table_ptr->ip_addr == ip_to_search)
            return arp_table_ptr;
        else
            arp_table_ptr = arp_table_ptr->next;
    }
    return NULL;
}

int tw_add_arp_entry(uint32_t ip_to_add, struct ether_addr mac_to_add, uint8_t port_id) {
    struct arp_table * arp_table_ptr = arp_table_root;
    if (arp_table_ptr == NULL) {
        arp_table_ptr = rte_malloc("struct arp_table", sizeof (struct arp_table), RTE_CACHE_LINE_SIZE);
        arp_table_root = arp_table_ptr;
    } else {
        while (arp_table_ptr->next != NULL)
            arp_table_ptr = arp_table_ptr->next;
        arp_table_ptr->next = rte_malloc("struct arp_table", sizeof (struct arp_table), RTE_CACHE_LINE_SIZE);
        arp_table_ptr = arp_table_ptr->next;
    }
    if (arp_table_ptr == NULL)
        rte_exit(EXIT_FAILURE, "CAN'T ALLOCATE ARP TABLE ENTRY\n");

    arp_table_ptr->ip_addr = rte_be_to_cpu_32(ip_to_add);
    ether_addr_copy(&(mac_to_add), &(arp_table_ptr->eth_mac));
    arp_table_ptr->port_id = port_id;
    arp_table_ptr->next = NULL;
    arp_table_size++;
    return 0;
}

int tw_send_arp_request(uint32_t ip_addr, char * port_name) {
    int port_id = tw_eth_name_to_id(port_name);
    tw_construct_arp_packet((ip_addr), port_id);
    return 0;
}

struct ether_addr * tw_search_arp_entry(char * ip_string) {
    uint32_t ip_addr = tw_convert_ip_str_to_dec(ip_string);
    struct arp_table * arp_entry = tw_search_arp_table(rte_cpu_to_be_32(ip_addr));
    if(arp_entry == NULL) {
        return NULL;
    }
    return &(arp_entry->eth_mac);
}

int tw_construct_arp_packet(uint32_t ip, uint8_t port_id) {
    int socket_id = rte_eth_dev_socket_id(port_id);

    if (socket_id == -1)
        socket_id = 0;
    struct rte_mbuf * m = rte_pktmbuf_alloc(tx_mempool[socket_id]);
    rte_pktmbuf_append(m, sizeof (struct arp_hdr));
    struct arp_hdr * arp_pkt = rte_pktmbuf_mtod(m, struct arp_hdr *);
    arp_pkt->arp_op = rte_cpu_to_be_16(ARP_OP_REQUEST);
    arp_pkt->arp_pro = rte_cpu_to_be_16(ETHER_TYPE_IPv4);
    arp_pkt->arp_pln = 4;
    arp_pkt->arp_hln = 6;
    arp_pkt->arp_hrd = rte_cpu_to_be_16(ARP_HRD_ETHER);
    ether_addr_copy(port_info[port_id].eth_mac, &(arp_pkt->arp_data.arp_sha));
    ether_addr_copy(&(querycastmac), &(arp_pkt->arp_data.arp_tha));
    arp_pkt->arp_data.arp_sip = rte_cpu_to_be_32(port_info[port_id].start_ip_addr);
    arp_pkt->arp_data.arp_tip = rte_cpu_to_be_32(ip);

    rte_pktmbuf_prepend(m, sizeof ( struct ether_hdr));
    struct ether_hdr* eth = rte_pktmbuf_mtod(m, struct ether_hdr *);
    eth->ether_type = rte_cpu_to_be_16(ETHER_TYPE_ARP);
    ether_addr_copy(port_info[port_id].eth_mac, &(eth->s_addr));
    ether_addr_copy(&(broadcastmac), &(eth->d_addr));
    tw_add_pkt_to_tx_queue(m, port_id);
    return 0;
}

void tw_print_arp_table(void) {
    printf("\nARP table\n");
    printf("\n|----------IP--------|----------MAC-------------|");
    struct in_addr ip_addr;
    struct arp_table * temp_arp_entry = arp_table_root;
    while (temp_arp_entry != NULL) {
        ip_addr.s_addr = tw_cpu_to_be_32(temp_arp_entry->ip_addr);
        printf("\n|    %s ", inet_ntoa(ip_addr));
        printf("  |     %02X:%02X:%02X:%02X:%02X:%02X    |",
                temp_arp_entry->eth_mac.addr_bytes[0],
                temp_arp_entry->eth_mac.addr_bytes[1],
                temp_arp_entry->eth_mac.addr_bytes[2],
                temp_arp_entry->eth_mac.addr_bytes[3],
                temp_arp_entry->eth_mac.addr_bytes[4],
                temp_arp_entry->eth_mac.addr_bytes[5]);
        temp_arp_entry = temp_arp_entry->next;
    }
    printf("\n");
    return;

}

inline int tw_arp_parser(tw_buf_t * buffer, char * port_name) {
    return tw_parse_arp(buffer->pkt, tw_eth_name_to_id(port_name));
}


