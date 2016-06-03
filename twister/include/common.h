#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <rte_byteorder.h>
#include <tw_common.h>

uint8_t tw_str_len(const char *);
void tw_slice_array(const char *, char *, uint8_t, uint8_t);
uint8_t tw_find_char(const char *, int *, char, uint8_t);
int tw_power(uint8_t, uint8_t);

#endif
