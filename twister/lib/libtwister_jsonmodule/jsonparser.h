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

char * registerfile (const char *); /**< registerfile - To use a JSON file, the file has to be registered this function register the JSON file. */
cJSON * parse_json_file(const char *); /**< parse_json_file - This function parse the JSON file. */
int get_port_conf_json_vals(const char *); /**< get_port_conf_json_vals - This function parse the JSON file containing port configuration. */
int get_lcore_queue_conf_json_vals(const char *); /**< get_lcore_queue_conf_json_vals - This function parse the JSON file containing logical core and queue configuration. */
int get_processing_conf_json_vals(const char *); /**< get_processing_conf_json_vals - This function parse the JSON file containing processing type model configuration. */

#endif
