#ifndef _INITFUNCS_H_
#define _INITFUNCS_H_

#include <rte_eal.h>
#include <lcoreconf.h>
#include <mempoolconf.h>
#include <periodic_timers.h>
#include <queued_pkts.h>

int init_eal_env(int, char **);
int parse_twister_args(void);
int init_global(int argc, char **);
int init_user_given_vals(void);

#endif
