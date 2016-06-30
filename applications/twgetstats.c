#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <stdint.h>
#include <tw_common.h>
#include <tw_api.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include<signal.h>
#include<unistd.h>

int main(int argc, char **argv)
{
    int stdout_dupfd;
    FILE *temp_out;

    stdout_dupfd = dup(1);
    temp_out = fopen("/dev/null", "w");
    dup2(fileno(temp_out), 1);
    //output redirected
	
	 tw_init_global();
    
    //restore output
    fflush(stdout);
    fclose(temp_out);
    dup2(stdout_dupfd, 1);
    close(stdout_dupfd);
    tw_get_port_stats (argv[1]);
    return 0;
	
}

