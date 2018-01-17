/**
* @file - debug.c
* @brief - Implementation file for the memory dump on stdio in DEBUG mode
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 02/10/2017
**/

#include "debug.h"
#ifdef VERBOSE
#include <stdio.h>
#endif

void print_memory(uint8_t *start, uint32_t length)
{
#ifdef VERBOSE
		uint32_t tempLength = length;
		printf("0x");
		while (tempLength--)
		{
			printf("%x", *start++);
		}
		printf("/n");
#endif
}
