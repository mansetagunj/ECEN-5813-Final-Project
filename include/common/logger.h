/**
* @file - logger.h
* @brief - Header file for the functions for the log functionality
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 8 Dec 2017
**/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "logger_helper.h"

/**
* @brief - Stores the state of logging.
* 			0 - to stop logging
* 			1 - to start/continue logging
*/
volatile uint8_t logging;

/**
* @brief - Stores the format in which the logging should occur
*/
volatile LOG_FORMAT_t log_format;

/**
* @brief - Similar to printf which takes the format and arguments which attaches this payload
* 			to the specified log id and it gets logged
* @param - LOG_ID_t
* @param - char *
* @param - variable argument
* @return void
**/
void logger_log(LOG_ID_t log_id, char *fmt, ...);

#endif /* __LOGGER_H__ */
