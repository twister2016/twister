/** @file Contains commonly used, helpful functions.
 *
 */

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <rte_byteorder.h>
#include <tw_common.h>

/** Finds the length of a string
 *
 * @param str :Pointer to string
 *
 * @return Length of string str
 */
uint8_t tw_str_len(const char * str);

/**
 * @param srcStr :String to slice
 *
 * @param dstStr :Pointer to new string
 *
 * @param start :Start point of slice
 *
 * @param dstLength :Length of slice
 *
 * @return
 */
void tw_slice_array(const char * srcStr, char * dstStr, uint8_t start, uint8_t dstLength);

/**
 * @param str :String to search
 *
 * @param quoteLoc :
 *
 * @param c :Character to search
 *
 * @param instance : Number of occurrences
 * @return
 */
uint8_t tw_find_char(const char * str, int * quoteLoc, char c, uint8_t instance);

/** Evaluates base to the exponent
 *
 * @param base :Base
 *
 * @param exp :Exponent
 *
 * @return Result of calculation
 */
int tw_power(uint8_t base, uint8_t exp);

#endif
