#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define KGRN  "\x1B[32m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

void print_pass(char * msg)
{
    printf("%s", msg);
    printf(KGRN);
    printf("%s", "PASS\n");
    printf(KNRM);
}

void print_fail(char * msg)
{
    printf("%s", msg);
    printf(KRED);
    printf("%s", "FAIL\n");
    printf(KNRM);
}

int main(int argc, char **argv)
{
    if(getuid())
    {
        printf("%s",
               "Permission denied: Please run the test using sudo to use Hugepages!\n");
        exit(0);
    }

    else if(tw_smoke_init_eal_env(argc, argv) != 0)
    {
        print_fail("Twister:    Initialize EAL - ");
        //printf("%s", "Twister:  Initialize EAL - FAIL\n");
        return -1;
    }

    else
    {
        print_pass("Twister:    Initialze EAL - ");
        //printf("Twister:  Initialize EAL - PASS\n");
    }

    if(tw_smoke_create_rx_tx_mempools() != 0)
    {
        //printf("%s", "Twister:  Create rx tx mempools - FAIL\n");
        print_fail("Twister:    Create rx tx mempools - ");
        return -1;
    }

    else
    {
        //printf("%s", "Twister:  Create rx tx mempools - PASS\n");
        print_pass("Twister:    Create rx tx mempools - ");
    }

    if(tw_lcore_conf_init() != 0)
    {
        //printf("%s", "Twister:  Init lcore conf - FAIL\n");
        print_fail("Twister:    Init lcore conf - ");
        return -1;
    }

    else
    {
        //printf("%s", "Twister:  Init lcore conf - PASS\n");
        print_pass("Twister:    Init lcore conf - ");
    }

    if(tw_smoke_eth_port_init() != 0)
    {
        //printf("%s", "Twister:  Init Ethernet ports - FAIL\n");
        print_fail("Twister:    Init Ethernet ports - ");
        return -1;
    }

    else
    {
        //printf("%s", "Twister:  Init Ethernet ports - PASS\n");
        print_pass("Twister:    Init Ethernet ports - ");
    }

    return 0;
}
