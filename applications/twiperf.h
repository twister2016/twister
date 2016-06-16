#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <getopt.h>
#include <tw_common.h>
#include <tw_api.h>
#include <arpa/inet.h>
#include <stats.h>
#include <signal.h>
/* Maximum size UDP send is (64K - 1) - IP and UDP header sizes */
//#define MAX_UDP_BLOCKSIZE (65535 - 8 - 20)
#define MAX_UDP_BLOCKSIZE 1500
const

const char stats_head[] =
"\n   Interval               RX pkts/s  TX pkts/s       Transfer      Bandwidth        Datagrams Send       Datagrams Recv";
const char summary_head[]=
"\n   Interval               Transfer         Bandwidth        Total Datagrams Send        Total Datagrams Recv";
const char stats_number[]=
"\n %6.2f-%-6.2f  sec     %7llu       %7llu      %5llu KBytes   %7.2f Mbits/sec     %7llu         %7llu" ;
const char summary_stats_number[]=
"\n %6.2f-%-6.2f  sec     %5llu KBytes   %7.2f Mbits/sec     %9llu      %20llu" ;
const char on_host_conn[]=
"Connecting to host %s, port %u\n";
const char summary_dot_line[]=
"\n- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -";

enum
{
    IENONE = 0,             // No error
    /* Parameter errors */
    IEUDPBLOCKSIZE = 1, //UDP block size exceeds
    PORTNUMBERWRONG = 2,           // Iperf must either be a client (-c) or server (-s)
    IENOROLE = 3,           // Iperf must either be a client (-c) or server (-s)
};

struct iperf_test
{
        int role; //1 for server, 2 for client
        uint32_t protocol_id; // 1 for ethernet, 2 for udp
        uint32_t server_port;
        uint32_t client_port;
        uint32_t server_ip;
        uint32_t client_ip;
        uint32_t packet_size;
        struct ether_addr * server_mac;
        struct ether_addr * client_mac;
        struct ether_hdr * eth;
        struct ipv4_hdr * ip;
        struct udp_hdr * udp;
        uint8_t test_runtime;
        tw_buf_t * tx_buf;
};

struct iperf_stats
{
        uint64_t datagrams_sent;
        uint64_t datagrams_recv;
        uint64_t total_transfered_bytes;
        float bandwidth;
        float interval_window;
};
/* display usage */
const char usage_longstr[] = "Usage: twiperf [-s|-c host] [options]\n"
        "       twiperf [-h|--help]\n\n"
        "  -h, --help                show this message and quit\n"
        "Server specific:\n"
        "  -s, --server              run in server mode\n"
        "Client specific:\n"
        "  -c, --client    <host>    run in client mode, connecting to <host>\n"
        "  -n, --bytes     <size>    Packet size in bytes. Default is 1470 bytes \n";

char * iperf_strerror(int i_errno)
{
    static char errstr[256];
    int len, perr, herr;
    perr = herr = 0;
    len = sizeof(errstr);
    memset(errstr, 0, len);

    switch (i_errno)
    {
        case IENONE:
            snprintf(errstr, len, "no error");
            break;
        case IEUDPBLOCKSIZE:
            snprintf(errstr, len, "block size too large (maximum = %d bytes)", MAX_UDP_BLOCKSIZE);
            break;
        case IENOROLE:
            snprintf(errstr, len, "must either be a client (-c) or server (-s)");
            break;
    }

    return errstr;
}

//void iperf_err(struct iperf_test *test, const char *format, ...) __attribute__ ((format(printf,2,3)));
void iperf_err(struct iperf_test *test, const char *format, ...)
{
    va_list argp;
    char str[1000];
    va_start(argp, format);
    vsnprintf(str, sizeof(str), format, argp);
    fprintf(stderr, "twiperf: %s\n", str);
    va_end(argp);
}

void twiperf_usage()
{
    fprintf(stderr, usage_longstr);
}

int twiprintf(struct iperf_test *test, const char* format, ...) //__attribute__ ((format(printf,2,3)))
{
    va_list argp;
    int r = -1;
    char linebuffer[1024];
    va_start(argp, format);
    r = vsnprintf(linebuffer, sizeof(linebuffer), format, argp);
    va_end(argp);
    fprintf(stdout, "%s", linebuffer);

    return r;
}
