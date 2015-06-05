#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


uint8_t convert_str_to_iparr(char *, uint8_t *);
uint32_t convert_str_to_hex(char *, uint8_t);
void ipDecToArr( uint32_t, uint8_t *);
uint32_t ip_arr_To_dec(uint8_t *);
uint32_t convert_ip_str_to_dec(char *);
uint8_t strLen(const char *);
void sliceArray(const char *, char *, uint8_t, uint8_t);
uint8_t findChar(const char *, int *, char, uint8_t);
uint64_t strToInt (const char *str, uint8_t n);
int power(uint8_t, uint8_t);

#endif
