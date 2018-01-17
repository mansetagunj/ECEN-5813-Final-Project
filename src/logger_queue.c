///*
// * logger_queue.c
// *
// *  Created on: 04-Dec-2017
// *      Author: Gunj Manseta
// */
//
//
//#include "logger_queue.h"
//#include "memory.h"
//#include <malloc.h>
//
//#define LOGGERQ_STATUS_CODE_COUNT 10
//
//const uint8_t * LOGGERQ_Status_Strings[LOGGERQ_STATUS_CODE_COUNT] = {
//(uint8_t *)"Operation Successful",
//(uint8_t *)"Logger Queue is Empty",
//(uint8_t *)"Logger Queue has some data",
//(uint8_t *)"Logger Queue is Full",
//(uint8_t *)"Logger Queue is not Full",
//(uint8_t *)"Some NULL pointer Error",
//(uint8_t *)"Logger Queue is not allocated memory",
//(uint8_t *)"Logger Queue allocation failure",
//};
//
//LOGGERQ_Status_t LOGGERQ_init(Logger_Queue_t *loggerQ, uint16_t length)
//{
//	LOGGERQ_Status_t returnStatus = LOGGERQ_SUCCESS;
//
//	if (NULL == loggerQ)
//		returnStatus = LOGGERQ_NULL_POINTER_ERROR;
//	else
//	{
//		loggerQ->buffer = (loggerQElement_t*)malloc(sizeof(loggerQElement_t)*length);
//		if (NULL == loggerQ->buffer)
//		{
//			loggerQ->size = 0;
//			loggerQ->head = NULL;
//			loggerQ->tail = NULL;
//			loggerQ->count = 0;
//			returnStatus = LOGGERQ_BUFFER_ALLOCATION_FAILURE;
//		}
//		else
//		{
//			loggerQ->size = length;
//			loggerQ->head = loggerQ->buffer;
//			loggerQ->tail = loggerQ->buffer;
//			loggerQ->count = 0;
//			my_memzero((uint8_t*)loggerQ->buffer, sizeof(*loggerQ->buffer));
//		}
//	}
//
//	return returnStatus;
//}
//
//
//LOGGERQ_Status_t LOGGERQ_buffer_add_item(Logger_Queue_t *loggerQ, loggerQElement_t dataToAdd)
//{
//	LOGGERQ_Status_t returnStatus = LOGGERQ_is_full(loggerQ);
//
//	if (LOGGERQ_BUFFER_FULL == returnStatus || LOGGERQ_BUFFER_NOT_ALLOCATED == returnStatus || LOGGERQ_NULL_POINTER_ERROR == returnStatus)
//		return returnStatus;
//	else
//	{
//		*(loggerQ->head) = dataToAdd;
//		loggerQ->count++;
//		if ((loggerQ->head - loggerQ->buffer) < (loggerQ->size - 1))	//there is still buffer location empty in the buffer, so we can move the head to the next buffer location
//			loggerQ->head++;
//		else
//			loggerQ->head = loggerQ->buffer;	//rolling back the head to the front of the buffer
//
//		returnStatus = LOGGERQ_SUCCESS;
//	}
//	return returnStatus;
//}
//
//LOGGERQ_Status_t LOGGERQ_buffer_remove_item(Logger_Queue_t *loggerQ, loggerQElement_t *outData)
//{
//	LOGGERQ_Status_t returnStatus = LOGGERQ_is_empty(loggerQ);
//
//	if (LOGGERQ_BUFFER_EMPTY == returnStatus || LOGGERQ_BUFFER_NOT_ALLOCATED == returnStatus || LOGGERQ_NULL_POINTER_ERROR == returnStatus)
//		return returnStatus;
//	else
//	{
//		*outData = *(loggerQ->tail);
//		my_memset((uint8_t*)loggerQ->tail,sizeof(*(loggerQ->tail)),0);
//		(loggerQ->count)--;
//		if ((loggerQ->tail - loggerQ->buffer) < (loggerQ->size - 1))	//there is still buffer location empty in the buffer, so we can move the tail to the next buffer location
//			loggerQ->tail++;
//		else
//			loggerQ->tail = loggerQ->buffer;	//rolling back the tail to the front of the buffer
//
//		returnStatus = LOGGERQ_SUCCESS;
//	}
//	return returnStatus;
//}
//
//LOGGERQ_Status_t LOGGERQ_peek(Logger_Queue_t * loggerQ, uint16_t position, loggerQElement_t *outPeekData)
//{
//	LOGGERQ_Status_t returnStatus = LOGGERQ_is_empty(loggerQ);
//
//	if (LOGGERQ_BUFFER_EMPTY == returnStatus || LOGGERQ_BUFFER_NOT_ALLOCATED == returnStatus || LOGGERQ_NULL_POINTER_ERROR == returnStatus)
//		return returnStatus;
//	else
//	{	/*Since the position right next to head is supposed to be empty, this function implementation will navigate 'position'
//		  no. of items before head to do a peek*/
//		if (loggerQ->count < position)	//Can't move through more than 'count' positions
//			returnStatus = LOGGERQ_NULL_POINTER_ERROR;
//		else if ((loggerQ->head - loggerQ->buffer)>(loggerQ->count)) {	//No need to wrap around
//			*outPeekData=*(loggerQ->head - position);
//			returnStatus = LOGGERQ_SUCCESS;
//		}
//		else {	//Need to wrap around to move through 'position' no. of items
//			*outPeekData= *(loggerQ->head + (loggerQ->size - position +1));
//			returnStatus = LOGGERQ_SUCCESS;
//		}
//	}
//	return returnStatus;
//}
//
//LOGGERQ_Status_t LOGGERQ_is_full(Logger_Queue_t *loggerQ)
//{
//	LOGGERQ_Status_t returnStatus = LOGGERQ_BUFFER_NOT_FULL;
//
//	if (NULL == loggerQ)
//		returnStatus = LOGGERQ_NULL_POINTER_ERROR;
//	else if (NULL == loggerQ->buffer)
//		returnStatus = LOGGERQ_BUFFER_NOT_ALLOCATED;
//	else
//	{
//		if (loggerQ->size == loggerQ->count)
//			returnStatus = LOGGERQ_BUFFER_FULL;
//	}
//
//	return returnStatus;
//}
//
//LOGGERQ_Status_t LOGGERQ_is_empty(Logger_Queue_t *loggerQ)
//{
//	LOGGERQ_Status_t returnStatus = LOGGERQ_BUFFER_NOT_EMPTY;
//
//	if (NULL == loggerQ)
//		returnStatus = LOGGERQ_NULL_POINTER_ERROR;
//	else if (NULL == loggerQ->buffer)
//		returnStatus = LOGGERQ_BUFFER_NOT_ALLOCATED;
//	else
//	{
//		if (0 == loggerQ->count)
//			returnStatus = LOGGERQ_BUFFER_EMPTY;
//	}
//
//	return returnStatus;
//}
//
//LOGGERQ_Status_t LOGGERQ_destroy(Logger_Queue_t *loggerQ)
//{
//	LOGGERQ_Status_t returnStatus = LOGGERQ_SUCCESS;
//
//	if (NULL == loggerQ)
//		returnStatus = LOGGERQ_NULL_POINTER_ERROR;
//	else
//	{
//		if (NULL != loggerQ->buffer)
//		{
//			free(loggerQ->buffer);
//			loggerQ->buffer = NULL;
//		}
//		loggerQ->size = 0;
//		loggerQ->head = NULL;
//		loggerQ->tail = NULL;
//		loggerQ->count = 0;
//	}
//
//	return returnStatus;
//}
//
//
//const uint8_t* get_LOGGERQ_error_String(LOGGERQ_Status_t cbStatusEnum)
//{
//	return LOGGERQ_Status_Strings[cbStatusEnum];
//}
