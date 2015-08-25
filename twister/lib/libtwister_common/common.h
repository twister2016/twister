#ifndef _COMMON_H_
#define _COMMON_H_
/**
 * @file
 *
 * Common Helpers in Twister
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>


uint8_t convert_str_to_iparr(const char *, uint8_t *); /**< convert_str_to_iparr - This function converts the IP address in string format to Array . */
uint32_t convert_str_to_hex(char *, uint8_t); /**< convert_str_to_hex - This functions convert string to hexadecimal. */
void ipDecToArr( uint32_t, uint8_t *); /**< ipDecToArr - This functions convert the IP address from decimal to Array. */
uint32_t ip_arr_To_dec(uint8_t *); /**< ip_arr_To_dec - This functions convert the IP address from array format to Decimal. */
uint32_t convert_ip_str_to_dec(const char *); /**< convert_ip_str_to_dec - This functions convert the IP address from string format to Decimal. */
uint8_t strLen(const char *); /**< strLen - This function returns the length of the string. */
void sliceArray(const char *, char *, uint8_t, uint8_t);  /**< sliceArray - This function slice the array. */
uint8_t findChar(const char *, int *, char, uint8_t); /**< findChar - This function find char in the array. */
uint64_t convert_str_to_int(const char *str, uint8_t n); /**< convert_str_to_int - This function convert the string to integer. */
int power(uint8_t, uint8_t); /**< power - This function takes power of two numbers. */

#endif
