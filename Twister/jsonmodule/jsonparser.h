#ifndef _JSONPARSER_H_
#define _JSONPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <portconf.h>
#include <common.h>
#include "cJSON.h"

char * registerfile (const char *);
cJSON * parse_json_file(const char *);
int get_port_json_values(const char *);

#endif
