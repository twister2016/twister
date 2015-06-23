#ifndef _INITFUNCS_H_
#define _INITFUNCS_H_

#include <getopt.h>
#include <rte_config.h>
#include <rte_eal.h>
#include <lcoreconf.h>
#include <mempoolconf.h>
#include <periodic_timers.h>
#include <queued_pkts.h>
#include <jsonparser.h>

extern int init_eal_env(int, char **);
extern int parse_twister_args(int argc, char **argv);
extern int init_global(int argc, char **);
extern int init_user_given_vals(void);
int display_usage(const char *);
int parse_portmask(const char *);

#endif
