#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    if(getuid())
    {
        printf("%s",
               "Permission denied: Please run the test using sudo to use Hugepages!\n");
        exit(0);
    }

    return tw_run_smoke();
}
