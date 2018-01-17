/**
* @file - memory.h
* @brief - Header file for the memory functions
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 02/10/2017
**/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdint.h>
#include <stdlib.h>

uint8_t dma_setValue;

/**
* @brief - Moves the data to/from memory location
* Moves the data from Source memory location to the Destination memory location.
* The function handles the overlap of the memory location.
* @param - src uint8_t*
* @param - dst uint8_t*
* @param - length size_t
* @return uint8_t*
**/
uint8_t* my_memmove(uint8_t *src, uint8_t *dst, size_t length);

/**
* @brief - Copies the data to/from memory location
* Copies the data from Source memory location to the Destination memory location.
* The function does not handle the overlap of the memory location.
* @param - src uint8_t*
* @param - dst uint8_t*
* @param - length size_t
* @return uint8_t*
**/
uint8_t* my_memcpy(uint8_t *src, uint8_t *dst, size_t length);

/**
* @brief - Sets the data in the given memory location with the given input unsigned int value
* 
* @param - src uint8_t*
* @param - length size_t
* @param - value uint8_t
* @return int8_t*
**/
int8_t* my_memset(uint8_t *src, size_t length, uint8_t value);

/**
* @brief - Sets the data in the given memory location with ZERO
*
* @param - src uint8_t*
* @param - length size_t
* @return uint8_t*
**/
uint8_t* my_memzero(uint8_t *src, size_t length);

/**
* @brief - Reverses the data within the memory location
*
* @param - src uint8_t*
* @param - length size_t
* @return uint8_t*
**/
uint8_t* my_reverse(uint8_t *src, size_t length);

/**
* @brief - Allocates memory of specified length and returns the pointer to the start of memory block
* 
* @param - length size_t
* @return int32_t*
**/
int32_t* reserve_words(size_t length);

/**
* @brief - Deallocates the memory pointed by the given pointer
* 
* @param - src int32_t*
* @return voide
**/
void free_words(int32_t *src);


int8_t memmove_dma(uint8_t *src, uint8_t *dst,size_t length);
int8_t memset_dma(uint8_t *src, size_t length, uint8_t value);


#endif
