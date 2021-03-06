/** @file Contains an enum and a single function to return core ID
 *
 */
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <rte_launch.h>
#include <rte_lcore.h>
#include <rte_ip.h>
#include <tw_api.h>
enum
{
    USE_ALL_ENGINES = 255, ENGINE0 = 0,    //TODO use string in this case
    ENGINE1 = 1,
    ENGINE2 = 2,
    ENGINE3 = 3,
    RESERVE_FIRST_ENGINE = 254
};

/** Returns the current core ID
 *
 * @return Core ID
 */
unsigned tw_current_engine(void);

#endif
