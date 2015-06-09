#include <stdio.h>
#include <stdint.h>
#include "../include/initfuncs.h"


int main(int argc, char **argv ) {
	printf("Hello World\n");
	//fuckit();
	int x=init_eal_env(argc, argv);
	return 0;
}
