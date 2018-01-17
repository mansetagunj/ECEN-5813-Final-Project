/**
* @file - logger_helper.h
* @brief - Header file for the helper functions for the log functionality
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 8 Dec 2017
**/

#ifndef __LOGGER_HELPER_H__
#define __LOGGER_HELPER_H__

#include <stdint.h>

#define LOG_TYPE_NUM 18


/**
* @brief - Enum for LOG ID
*/
typedef enum
{
	HEARTBEAT = 0,			//No Payload
	LOGGER_INITIALZED,		//No Payload
	GPIO_INITIALZED, 		//No Payload
	SYSTEM_INITIALIZED,		// No Payload
	SYSTEM_HALTED, 			//No Payload
	INFO, 					//Sends important information with regards to an info string
	WARNING, 				//Sends important information with regards to an info string
	ERROR, 					//Sends important information with regards to an info string
	PROFILING_STARTED, 		//Profiling analysis has started
	PROFILING_RESULT, 		//Logs a function identifier and a count for how long it took
	PROFILING_COMPLETED,	//Profiling analysis has completed
	DATA_RECEIVED, 			//Logs that an item was received on the UART, transmits it back
	DATA_ANALYSIS_STARTED, 	//No Payload
	DATA_ALPHA_COUNT, 		//Logs number of alphabetic characters
	DATA_NUMERIC_COUNT, 	//Logs number of alphabetic characters
	DATA_PUNCTUATION_COUNT, //Logs number of alphabetic characters
	DATA_MISC_COUNT, 		//Logs number of alphabetic characters
	DATA_ANALYSIS_COMPLETED	//No Payload
}LOG_ID_t;

/**
* @brief - Structure of the LOG item
*/
typedef struct
{
	LOG_ID_t	logId;
	uint32_t	timeStamp;
	uint32_t	payloadSize;
	uint8_t		*payload;
	uint8_t		checksum;
}log_t;

/**
* @brief - Enum for Log format
*/
typedef enum{
	BINARY_LOGGER=0,
	ASCII_LOGGER
}LOG_FORMAT_t;

/**
* @brief - Function pointer vector to pick up the respective function depending on the LOG_ID
*/
log_t* (*const log_vector[LOG_TYPE_NUM])(char*,uint32_t);

#ifdef VERBOSE

#ifdef PLATFORM_KL25Z

#include "uart0.h"

#define LOG_RAW_DATA(data,len)			UART0_CBsendN(data,len)
#define LOG_RAW_STRING(string)			UART0_putstr(string)
#define LOG_RAW_INT(int_data)			UART0_printf("%d",int_data)
#define LOG_RAW_FLOAT(float_data)		UART0_printf("%f",float_data)
#define LOG_FLUSH()						UART0_logFlush();

#else

#include <stdio.h>

#define LOG_RAW_DATA(data,len)			printf("%.*s",(int)len,data)
#define LOG_RAW_STRING(string)			printf("%s",string)
#define LOG_RAW_INT(int_data)			printf("%u",(int)int_data)
#define LOG_RAW_FLOAT(float_data)		printf("%f",float_data)
#define LOG_FLUSH()

#endif

#else

#define LOG_RAW_DATA(data,len)
#define LOG_RAW_STRING(string)
#define LOG_RAW_INT(int_data)
#define LOG_RAW_FLOAT(float_data)
#define LOG_FLUSH()

#endif

/**
* @brief - logs the contents in the log structure in the specified format
* @param - log_t*
* @param - LOG_FORMAT_t
* @return void
**/
void log_item(log_t *log_item, LOG_FORMAT_t format);


#endif /* __LOGGER_HELPER_H__ */
