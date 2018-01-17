/*
 * data_processing.c
 *
 *  Created on: 05-Dec-2017
 *      Author: Gunj Manseta
 */

#include "logger.h"
#include <malloc.h>
#include "conversion.h"
#include "data_processing.h"

#ifndef __STATIC_INLINE
#define __STATIC_INLINE static inline
#endif

#define ALPHA_UPPER_START 	(0x40)
#define ALPHA_UPPER_END 	(0x5B)
#define ALPHA_LOWER_START	(0X60)
#define ALPHA_LOWER_END 	(0X7B)
#define NUM_START 			(0X2F)
#define NUM_END 			(0X3A)

#define MAX_STATISTICS_DATA 		(64)
#define TYPES_OF_STATISTICS_DATA	(4)

__STATIC_INLINE uint8_t is_alphabet(uint8_t val)
{
	if ((val>ALPHA_UPPER_START && val<ALPHA_UPPER_END) || (val>ALPHA_LOWER_START && val<ALPHA_LOWER_END))
		return 1;
	else
		return 0;
}
__STATIC_INLINE uint8_t is_numeric(uint8_t val)
{
	if (val>NUM_START && val<NUM_END)
		return 1;
	else
		return 0;
}
__STATIC_INLINE uint8_t is_punctuation(uint8_t val)
{
	switch ((unsigned char)val)
	{
	    case '.':
	    case '\'':
	    case '\"':
	    case ':':
	    case ';':
	    case ',':
	    case '?':
	    case '!': return 1;
	    default: return 0;
	}
}

void processData(CB_t *RXBuffer)
{
	logger_log(DATA_ANALYSIS_STARTED,NULL);

	CB_Status_t status = CB_is_empty(RXBuffer);
	if(CB_BUFFER_EMPTY == status || CB_NULL_POINTER_ERROR == status)
	{
		return;
	}

	uint8_t alphabets_count 		= 0;
	uint8_t numerics_count 			= 0;
	uint8_t punctuations_count 		= 0;
	uint8_t miscChar_count			= 0;
	uint8_t itr						= 0;
	uint8_t currentChar				= 0;

	while(CB_SUCCESS == CB_buffer_remove_item(RXBuffer,&currentChar) && (itr < MAX_STATISTICS_DATA))
	{

		if(is_alphabet(currentChar))
		{
			alphabets_count++;
		}
		else if(is_numeric(currentChar))
		{
			numerics_count++;
		}
		else if(is_punctuation(currentChar))
		{
			punctuations_count++;
		}
		else
		{
			miscChar_count++;
		}

	}

	logger_log(DATA_ALPHA_COUNT,"%d",alphabets_count);
	logger_log(DATA_NUMERIC_COUNT,"%d",numerics_count);
	logger_log(DATA_PUNCTUATION_COUNT,"%d",punctuations_count);
	logger_log(DATA_MISC_COUNT,"%d",miscChar_count);

	logger_log(DATA_ANALYSIS_COMPLETED,"");
}
