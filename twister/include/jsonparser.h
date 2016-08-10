/**@file Contains functions related to the json file.
 *
 */
#ifndef _JSONPARSER_H_
#define _JSONPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <lcoreconf.h>
#include <common.h>
#include "cJSON.h"
#include <tw_common.h>

/** Registers the json file and retrieves its raw data.
 *
 * @param filename :Name of file to register
 *
 * @return Raw data of file
 */
char * tw_registerfile(const char * filename);

/** Assigns IP to port
 *
 * @param file_name :Name of json file
 *
 * @return 0(TRUE), -1(FALSE)
 */
int tw_get_port_conf_json_vals(const char * file_name);


#endif
