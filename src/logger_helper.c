/*
 * logger_helper.c
 *
 *  Created on: 05-Dec-2017
 *      Author: Gunj Manseta
 */


#include "logger_helper.h"
#include <stdarg.h>
#include <stdint.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "timestamp.h"

const char* const LOG_ID_Strings[LOG_TYPE_NUM] = {
(const char *)"<HEARTBEAT>",
(const char *)"<LOGGER_INITIALZED>",
(const char *)"<GPIO_INITIALZED>",
(const char *)"<SYSTEM_INITIALIZED>",
(const char *)"<SYSTEM_HALTED>",
(const char *)"<INFO>",
(const char *)"<WARNING>",
(const char *)"<ERROR>",
(const char *)"<PROFILING_STARTED>",
(const char *)"<PROFILING_RESULT>",
(const char *)"<PROFILING_COMPLETED>",
(const char *)"<DATA_RECEIVED>",
(const char *)"<DATA_ANALYSIS_STARTED>",
(const char *)"<DATA_ALPHA_COUNT>",
(const char *)"<DATA_NUMERIC_COUNT>",
(const char *)"<DATA_PUNCTUATION_COUNT>",
(const char *)"<DATA_MISC_COUNT>",
(const char *)"<DATA_ANALYSIS_COMPLETED>",
};

log_t* log_hearbeat(char* payload, uint32_t len);
log_t* log_logger_initialized(char* payload, uint32_t len);
log_t* log_gpio_initialized(char* payload, uint32_t len);
log_t* log_system_initialized(char* payload, uint32_t len);
log_t* log_system_halted(char* payload, uint32_t len);
log_t* log_info(char* payload, uint32_t len);
log_t* log_warning(char* payload, uint32_t len);
log_t* log_error(char* payload, uint32_t len);
log_t* log_profiling_started(char* payload, uint32_t len);
log_t* log_profiling_result(char* payload, uint32_t len);
log_t* log_profiling_completed(char* payload, uint32_t len);
log_t* log_data_received(char* payload, uint32_t len);
log_t* log_data_analysis_started(char* payload, uint32_t len);
log_t* log_data_alpha_count(char* payload, uint32_t len);
log_t* log_data_numeric_count(char* payload, uint32_t len);
log_t* log_data_punctuation_count(char* payload, uint32_t len);
log_t* log_data_misc_count(char* payload, uint32_t len);
log_t* log_data_analysis_completed(char* payload, uint32_t len);

//function pointer tables to handle specific log events
log_t* (*const log_vector[LOG_TYPE_NUM])(char*,uint32_t) = {

		log_hearbeat,
		log_logger_initialized,
		log_gpio_initialized,
		log_system_initialized,
		log_system_halted,
		log_info,
		log_warning,
		log_error,
		log_profiling_started,
		log_profiling_result,
		log_profiling_completed,
		log_data_received,
		log_data_analysis_started,
		log_data_alpha_count,
		log_data_numeric_count,
		log_data_punctuation_count,
		log_data_misc_count,
		log_data_analysis_completed
};


uint8_t getChecksum(log_t* log_item, size_t log_item_size)
{
	//(in bytes)log_item_size - payload_pointer_size - checksum_size
	size_t data_size = log_item_size - 4 - 4;
	size_t payload_size = log_item->payloadSize;
	uint8_t checksum = 0;
	size_t itr = 0;
	while(itr < data_size)
	{
		checksum ^= ((uint8_t*)log_item)[itr++];
	}
	itr = 0;
	while(itr < payload_size)
	{
		checksum ^= *(log_item->payload+itr);
		itr++;
	}
	//srand(checksum);
	//return rand();
	return checksum;
}

log_t* log_hearbeat(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = HEARTBEAT;
	log_item->payload = NULL;
	log_item->payloadSize = 0;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_logger_initialized(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = LOGGER_INITIALZED;
	log_item->payload = NULL;
	log_item->payloadSize = 0;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_gpio_initialized(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = GPIO_INITIALZED;
	log_item->payload = NULL;
	log_item->payloadSize = 0;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_system_initialized(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = SYSTEM_INITIALIZED;
	log_item->payload = NULL;
	log_item->payloadSize = 0;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_system_halted(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = SYSTEM_HALTED;
	log_item->payload = NULL;
	log_item->payloadSize = 0;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_info(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = INFO;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}
log_t* log_warning(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = WARNING;
	log_item->payload = (uint8_t*)malloc(sizeof(uint8_t)*len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_error(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = ERROR;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_profiling_started(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = PROFILING_STARTED;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}
log_t* log_profiling_result(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = PROFILING_RESULT;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}
log_t* log_profiling_completed(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = PROFILING_COMPLETED;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_data_received(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = DATA_RECEIVED;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}

log_t* log_data_analysis_started(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = DATA_ANALYSIS_STARTED;
	log_item->payload = NULL;
	log_item->payloadSize = 0;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;

}
log_t* log_data_alpha_count(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = DATA_ALPHA_COUNT;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;
}
log_t* log_data_numeric_count(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = DATA_NUMERIC_COUNT;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;

}
log_t* log_data_punctuation_count(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = DATA_PUNCTUATION_COUNT;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;

}
log_t* log_data_misc_count(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = DATA_MISC_COUNT;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;

}
log_t* log_data_analysis_completed(char* payload, uint32_t len)
{
	log_t *log_item = (log_t*)malloc(sizeof(log_t));
	log_item->logId = DATA_ANALYSIS_COMPLETED;
	log_item->payload = (uint8_t*)malloc(len);
	log_item->payload = memmove(log_item->payload,payload,len);
	log_item->payloadSize = len;
	log_item->timeStamp = getTimeStamp();
	log_item->checksum = getChecksum(log_item, sizeof(*log_item));
	return log_item;

}
const char* get_LOG_ID_String(LOG_ID_t log_id)
{
	return LOG_ID_Strings[log_id];
}

void log_binary(log_t *log_item)
{
	if(log_item)
	{
		LOG_RAW_DATA((uint8_t*)log_item,4);	//sending LOG_ID
		LOG_RAW_DATA(((uint8_t*)log_item)+4,4);	//sending TIMESTAMP
		LOG_RAW_DATA(((uint8_t*)log_item)+8,4);	//sending PAYLOAD LEN
		LOG_RAW_DATA((uint8_t*)*(uint32_t*)(((uint8_t*)log_item)+12),log_item->payloadSize);	//sending PAYLOAD
		LOG_RAW_DATA(((uint8_t*)log_item)+16,1);	//sending CHECKSUM
	}
	else
		LOG_RAW_STRING("<INTERNAL ERROR> Log_item not found.\r\n\r\n");
}

void log_ascii(log_t *log_item)
{
	if(log_item)
	{
		LOG_RAW_INT(log_item->timeStamp);
//		LOG_RAW_STRING(getString_of_TimeStamp(log_item->timeStamp));
		LOG_RAW_STRING("\t");
		LOG_RAW_INT(log_item->logId);
		LOG_RAW_STRING(":");
		LOG_RAW_STRING(get_LOG_ID_String(log_item->logId));
		LOG_RAW_STRING(" ");
		LOG_RAW_DATA(log_item->payload,log_item->payloadSize);
//		LOG_RAW_STRING(" CS: ");
//		LOG_RAW_INT(log_item->checksum);
		LOG_RAW_STRING("\r\n\r\n");
	}
	else
		LOG_RAW_STRING("<INTERNAL ERROR> Log_item not found.\r\n\r\n");
}

void log_item(log_t *log_item, LOG_FORMAT_t format)
{
	if(format == BINARY_LOGGER)
		log_binary(log_item);
	else
		log_ascii(log_item);
}
