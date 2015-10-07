#ifndef _COMMON_H_
#define _COMMON_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <rte_byteorder.h>
#include <tw_common.h>



//uint8_t tw_convert_str_to_iparr(const char *, uint8_t *);
//uint32_t tw_convert_str_to_hex(char *, uint8_t);
//void tw_ip_dec_to_arr( uint32_t, uint8_t *);
//uint32_t tw_ip_arr_to_dec(uint8_t *);
//uint32_t tw_convert_ip_str_to_dec(const char *);
uint8_t tw_str_len(const char *);
void tw_slice_array(const char *, char *, uint8_t, uint8_t);
uint8_t tw_find_char(const char *, int *, char, uint8_t);
//uint64_t tw_convert_str_to_int(const char *str, uint8_t n);
int tw_power(uint8_t, uint8_t);
//inline uint16_t tw_be_to_cpu_16(uint16_t);
//inline uint16_t tw_cpu_to_be_16(uint16_t);
//inline uint32_t tw_cpu_to_be_32(uint32_t);
//uint8_t tw_parse_int_from_string(char*);*/

#endif
