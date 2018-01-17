/**
* @file - circular_buffer.c
* @brief - Implementation file for the circular buffer functionalities
* 
* @author Gunj/Ashish University of Colorado Boulder
* @date 27/10/2017
**/

#include "circular_buffer.h"
#include "memory.h"
#include <malloc.h>

#define CB_STATUS_CODE_COUNT 10

const uint8_t * const CB_Status_Strings[CB_STATUS_CODE_COUNT] = {
(uint8_t *)"Operation Successful",
(uint8_t *)"Buffer is Empty",
(uint8_t *)"Buffer has some data",
(uint8_t *)"Buffer is Full",
(uint8_t *)"Buffer is not Full",
(uint8_t *)"Some NULL pointer Error",
(uint8_t *)"Buffer is not allocated memory",
(uint8_t *)"Buffer allocation failure",
};

CB_Status_t CB_init(CB_t *cbuffer, uint16_t length)
{
	CB_Status_t returnStatus = CB_SUCCESS;

	if (NULL == cbuffer)
		returnStatus = CB_NULL_POINTER_ERROR;
	else
	{
		cbuffer->buffer = (bufferElement_t*)malloc(sizeof(bufferElement_t)*length);
		if (NULL == cbuffer->buffer)
		{
			cbuffer->size = 0;
			cbuffer->head = NULL;
			cbuffer->tail = NULL;
			cbuffer->count = 0;
			returnStatus = CB_BUFFER_ALLOCATION_FAILURE;
		}
		else
		{
			cbuffer->size = length;
			cbuffer->head = cbuffer->buffer;
			cbuffer->tail = cbuffer->buffer;
			cbuffer->count = 0;
			my_memzero(cbuffer->buffer, length);
		}
	}

	return returnStatus;
}


CB_Status_t CB_buffer_add_item(CB_t *cbuffer, bufferElement_t dataToAdd)
{
	CB_Status_t returnStatus = CB_is_full(cbuffer);

	if (CB_BUFFER_FULL == returnStatus || CB_BUFFER_NOT_ALLOCATED == returnStatus || CB_NULL_POINTER_ERROR == returnStatus)
		return returnStatus;
	else
	{
		*(cbuffer->head) = dataToAdd;
		cbuffer->count++;
		if ((cbuffer->head - cbuffer->buffer) < (cbuffer->size - 1))	//there is still buffer location empty in the buffer, so we can move the head to the next buffer location
			cbuffer->head++;
		else
			cbuffer->head = cbuffer->buffer;	//rolling back the head to the front of the buffer

		returnStatus = CB_SUCCESS;
	}
	return returnStatus;
}

CB_Status_t CB_buffer_remove_item(CB_t *cbuffer, bufferElement_t *outData)
{
	CB_Status_t returnStatus = CB_is_empty(cbuffer);

	if (CB_BUFFER_EMPTY == returnStatus || CB_BUFFER_NOT_ALLOCATED == returnStatus || CB_NULL_POINTER_ERROR == returnStatus)
		return returnStatus;
	else
	{
		*outData = *(cbuffer->tail);
		*(cbuffer->tail) = 0;
		(cbuffer->count)--;
		if ((cbuffer->tail - cbuffer->buffer) < (cbuffer->size - 1))	//there is still buffer location empty in the buffer, so we can move the tail to the next buffer location
			cbuffer->tail++;
		else
			cbuffer->tail = cbuffer->buffer;	//rolling back the tail to the front of the buffer

		returnStatus = CB_SUCCESS;
	}
	return returnStatus;
}

CB_Status_t CB_peek(CB_t * cbuffer, uint16_t position, bufferElement_t *outPeekData)
{
	CB_Status_t returnStatus = CB_is_empty(cbuffer);

	if (CB_BUFFER_EMPTY == returnStatus || CB_BUFFER_NOT_ALLOCATED == returnStatus || CB_NULL_POINTER_ERROR == returnStatus)
		return returnStatus;
	else
	{	/*Since the position right next to head is supposed to be empty, this function implementation will navigate 'position'
		  no. of items before head to do a peek*/
		if (cbuffer->count < position)	//Can't move through more than 'count' positions
			returnStatus = CB_NULL_POINTER_ERROR;
		else if ((cbuffer->head - cbuffer->buffer)>(cbuffer->count)) {	//No need to wrap around
			*outPeekData=*(cbuffer->head - position);
			returnStatus = CB_SUCCESS;
		}
		else {	//Need to wrap around to move through 'position' no. of items
			*outPeekData= *(cbuffer->head + (cbuffer->size - position +1));
			returnStatus = CB_SUCCESS;
		}
	}
	return returnStatus;
}

CB_Status_t CB_is_full(CB_t *cbuffer)
{
	CB_Status_t returnStatus = CB_BUFFER_NOT_FULL;

	if (NULL == cbuffer)
		returnStatus = CB_NULL_POINTER_ERROR;
	else if (NULL == cbuffer->buffer)
		returnStatus = CB_BUFFER_NOT_ALLOCATED;
	else
	{
		if (cbuffer->size == cbuffer->count)
			returnStatus = CB_BUFFER_FULL;
	}

	return returnStatus;
}

CB_Status_t CB_is_empty(CB_t *cbuffer)
{
	CB_Status_t returnStatus = CB_BUFFER_NOT_EMPTY;

	if (NULL == cbuffer)
		returnStatus = CB_NULL_POINTER_ERROR;
	else if (NULL == cbuffer->buffer)
		returnStatus = CB_BUFFER_NOT_ALLOCATED;
	else
	{
		if (0 == cbuffer->count)
			returnStatus = CB_BUFFER_EMPTY;
	}

	return returnStatus;
}

CB_Status_t CB_destroy(CB_t *cbuffer)
{
	CB_Status_t returnStatus = CB_SUCCESS;

	if (NULL == cbuffer)
		returnStatus = CB_NULL_POINTER_ERROR;
	else
	{
		if (NULL != cbuffer->buffer)
		{	
			free(cbuffer->buffer);
			cbuffer->buffer = NULL;
		}
		cbuffer->size = 0;
		cbuffer->head = NULL;
		cbuffer->tail = NULL;
		cbuffer->count = 0;
	}

	return returnStatus;
}


const uint8_t* get_CB_error_String(CB_Status_t cbStatusEnum)
{
	return CB_Status_Strings[cbStatusEnum];
}
