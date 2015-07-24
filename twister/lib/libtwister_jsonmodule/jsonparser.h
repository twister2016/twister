#ifndef _JSONPARSER_H_
#define _JSONPARSER_H_
/**
 * @file
 *
 * JSON parser Helpers in Twister
 */
#include <stdio.h>
#include <stdlib.h>
#include <lcoreconf.h>
#include <common.h>
#include "cJSON.h"

char * registerfile (const char *);
cJSON * parse_json_file(const char *);
int get_port_conf_json_vals(const char *);
int get_lcore_queue_conf_json_vals(const char *);
int get_processing_conf_json_vals(const char *);

#endif
