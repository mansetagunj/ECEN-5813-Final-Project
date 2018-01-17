/*
 * logger.c
 *
 *  Created on: 04-Dec-2017
 *      Author: Gunj Manseta
 */

#include "logger.h"
#include "logger_helper.h"
#include <stdarg.h>
#include <stdint.h>
#include <malloc.h>
#include <strings.h>
#include <stdio.h>

#ifdef VERBOSE
uint8_t verbose_flag = 1;
#else
uint8_t verbose_flag = 0;
#endif

volatile uint8_t logging = 1;
volatile LOG_FORMAT_t log_format = ASCII_LOGGER;

void logger_log(LOG_ID_t log_id, char *fmt, ...)
{
	if(logging && verbose_flag)
	{
		va_list args;
		va_start(args, fmt);
		char *payload =(char*)malloc(100);
		uint32_t len = vsnprintf(payload,100,fmt, args);
		va_end(args);
		log_t *log_struct = log_vector[log_id](payload,len+1);	//len+1 because the payload is null terminated string
		log_item(log_struct, log_format);
		free(payload);
		free(log_struct->payload);
		free(log_struct);
	}
}
