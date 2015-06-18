#include <stdio.h>
#include <stdint.h>
#include <initfuncs.h>

int main(int argc, char **argv ) {
	printf("Hello World\n");
	int x=init_global(argc, argv);
        return 0;
}
