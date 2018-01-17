/**
* @file - conversion.h
* @brief - Header file for the data conversion functions
* 
* @author Gunj/Ashish University of Colorado Boulder
* @date 02/10/2017
**/

#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#include <stdint.h>

#define BASE_2 (2)
#define BASE_10 (10)
#define BASE_16 (16)

/**
* @brief - Converts a given Signed Integer into ASCII char string as per the Base 2 through 16 
* Takes a signed integer, converts it into the format as per the given base and creates an ASCII char string
* Returns the length of the ASCII char string
* @param - data int32_t
* @param - ptr	uint8_t*
* @param - base	uint32_t
* @return uint8_t
**/
uint8_t my_itoa(int32_t data, uint8_t *ptr, uint32_t base);

/**
* @brief - Converts a given ASCII char string taking the BASE as the reference, to an signed integer
* 
* @param - ptr		uint8_t*
* @param - digits	uint8_t
* @param - base		uint32_t
* @return	int32_t
**/
int32_t my_atoi(uint8_t *ptr, uint8_t digits, uint32_t base);

/**
* @brief - Converts the data from Big Endian format to Little Endian format
* 
* @param - data uint32_t
* @param - length uint32_t
* @return int8_t
**/
int8_t big_to_little32(uint32_t *data, uint32_t length);

/**
* @brief - Converts the data from Little Endian format to Big Endian format
* 
* @param - data uint32_t
* @param - length uint32_t
* @return int8_t
**/
int8_t little_to_big32(uint32_t *data, uint32_t length);

#endif
