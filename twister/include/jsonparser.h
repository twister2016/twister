#ifndef _JSONPARSER_H_
#define _JSONPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <lcoreconf.h>
#include <common.h>
#include "cJSON.h"

char * tw_registerfile (const char *);
cJSON * tw_parse_json_file(const char *);
int tw_get_port_conf_json_vals(const char *);
int tw_get_lcore_queue_conf_json_vals(const char *);
int tw_get_processing_conf_json_vals(const char *);

#endif
