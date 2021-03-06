#include <stdio.h>
#include <sys/types.h>
#include <tw_api.h>
#include <tw_common.h>

#define INFINITE_LOOP 0
#define KGRN  "\x1B[32m"
#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"

tw_loop_t * tw_loop;

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

void tx_cb(tw_tx_t * handle, tw_buf_t * buffer)
{
    printf("Twister:    tx callback called\n");

    tw_tx_unregister(handle, tw_loop);
}

void rx_cb(tw_rx_t * handle, tw_buf_t * buffer)
{
    //printf("Twister:    rx callback called\n");
}

void timer_cb(tw_timer_t * handle, tw_buf_t * buffer)
{
    printf("Twister:    timer callback called\n");

    tw_timer_unregister(handle, tw_loop);
    exit(0);
}

int main(int argc, char **argv)
{
    if(getuid())
    {
        printf("%s", "Permission denied: Please run the test using sudo to use Hugepages!\n");
        exit(0);
    }

	//redirecting stdout
    int stdout_dupfd;
    FILE *temp_out;
    stdout_dupfd = dup(1);
    temp_out = fopen("/dev/null", "w");
    dup2(fileno(temp_out), 1);

    tw_init_global();
    tw_map_port_to_engine("tw0", "engine0");

	//restoring stdout
    fflush(stdout);
    fclose(temp_out);
    dup2(stdout_dupfd, 1);
    close(stdout_dupfd);

    return user_app_main(NULL);
}

int user_app_main(__attribute__((unused)) void * app_params)
{
    tw_tx_t     * tx_handle;
    tw_rx_t     * rx_handle;
    tw_timer_t  * timer_handle;
    tw_loop = tw_default_loop(INFINITE_LOOP);
    rx_handle = tw_rx_init(tw_loop);

    if(rx_handle == NULL)
    {
        printf("Twister:    Error in rx init\n");
        //printf("Twister:    Initialize rx - FAIL\n");
        print_fail("Twister:    Initialize rx - ");

        return -1;
        //exit(1);
    }

    else
    {
        //printf("Twister:    Initialize rx - PASS\n");
        print_pass("Twister:    Initialize rx - ");
    }

    int rx_status = tw_rx_start(rx_handle, rx_cb);

    if(rx_status)
    {
        //printf("Twister:    Register rx callback - FAIL\n");
        print_fail("Twister:    Register rx callback - ");

        return -1;
        //exit(1);
    }

    else
    {
        //printf("Twister:    Register rx callback - PASS\n");
        print_pass("Twister:    Register rx callback - ");
    }

    tx_handle = tw_tx_init(tw_loop);

    if(tx_handle == NULL)
    {
        //printf("Twister:    Initialize tx - FAIL\n");
        print_fail("Twister:    Initialize tx - ");

        return -1;
        //exit(1);
    }

    else
    {
        //printf("Twister:    Initialize tx - PASS\n");
        print_pass("Twister:    Initialize tx - ");

    }

    int tx_status = tw_tx_start(tx_handle, tx_cb);

    if(tx_status)
    {
        //printf("Twister:    Register tx callback - FAIL\n");
        print_fail("Twister:   Register tx callback - ");

        return -1;
        //exit(1);
    }

    else
    {
        print_pass("Twister:    Register tx callback - ");
        //printf("Twister:    Register tx callback - PASS\n");
    }

    timer_handle = tw_timer_init(tw_loop);

    if(timer_handle == NULL)
    {
        //printf("Twister:    Initialize timer - FAIL\n");
        print_fail("Twister:    Initialize timer - ");

        return -1;
        //exit(1);
    }

    else
    {
        //printf("Twister:    Initialize timer - PASS\n");
        print_pass("Twister:    Initialize timer - ");
    }

    int timer_status = tw_timer_start(timer_handle, timer_cb, 1000);

    if(timer_status)
    {
        //printf("Twister:    Register timer callback - FAIL\n");
        print_fail("Twister:    Register timer callback - ");

        return -1;
    }

    else
    {
        //printf("Twister:    Register timer callback - PASS\n");
        print_pass("Twister:    Register timer callback - ");
    }

    tw_run(tw_loop);

    return 0;
}
