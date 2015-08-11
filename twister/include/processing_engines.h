#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <rte_launch.h>

enum {
	USE_ALL_ENGINES = 0,
	RESERVE_FIRST_ENGINE = 1
};

void tw_launch_processing_engines(void *, void *, int);

#endif
