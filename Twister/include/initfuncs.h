#ifndef _INITFUNCS_H_
#define _INITFUNCS_H_

#include <rte_config.h>
#include <rte_eal.h>
#include <lcoreconf.h>
#include <mempoolconf.h>
#include <periodic_timers.h>
#include <queued_pkts.h>
#include <jsonparser.h>

extern int init_eal_env(int, char **);
extern int parse_twister_args(void);
extern int init_global(int argc, char **);
extern int init_user_given_vals(void);

#endif
