/**
* @file - debug.h
* @brief - Header file for the memory dump on stdio in DEBUG mode
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 02/10/2017
**/

#ifndef __DEBUGH_H__
#define __DEBUGH_H__

#include <stdint.h>

/**
 * @brief - Dumps the data in memory onto stdio
 * Takes a uint8_t pointer to the memory location and the length of the memory block and
 * dumps the memory data onto stdio
 * @param - start uint8_t*
 * @param - length uint32_t
 * @return void
**/
void print_memory(uint8_t *start, uint32_t length);

#endif
