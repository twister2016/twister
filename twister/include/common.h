#ifndef _COMMON_H_
#define _COMMON_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <rte_byteorder.h>



uint8_t convert_str_to_iparr(const char *, uint8_t *);
uint32_t convert_str_to_hex(char *, uint8_t);
void ipDecToArr( uint32_t, uint8_t *);
uint32_t ip_arr_To_dec(uint8_t *);
uint32_t convert_ip_str_to_dec(const char *);
uint8_t strLen(const char *);
void sliceArray(const char *, char *, uint8_t, uint8_t);
uint8_t findChar(const char *, int *, char, uint8_t);
uint64_t convert_str_to_int(const char *str, uint8_t n);
int power(uint8_t, uint8_t);
inline uint16_t tw_be_to_cpu_16(uint16_t);
inline uint16_t tw_cpu_to_be_16(uint16_t);
uint8_t parseIntFromString(char*);

#endif
