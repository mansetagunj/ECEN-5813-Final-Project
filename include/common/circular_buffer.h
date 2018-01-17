#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__
/**
* @file - circular_buffer.h
* @brief - Header file for the circular buffer implementation
* 
* @author Gunj/Ashish University of Colorado Boulder
* @date 27/10/2017
**/

#include <stdint.h>

typedef uint8_t bufferElement_t;
typedef uint8_t* p_bufferElement_t;

typedef enum CB_status{
	CB_SUCCESS,
	CB_BUFFER_EMPTY,
	CB_BUFFER_NOT_EMPTY,
	CB_BUFFER_FULL,
	CB_BUFFER_NOT_FULL,
	CB_NULL_POINTER_ERROR,
	CB_BUFFER_NOT_ALLOCATED,
	CB_BUFFER_ALLOCATION_FAILURE
}CB_Status_t;


typedef struct CB{
	bufferElement_t *buffer;
	bufferElement_t *head;
	bufferElement_t *tail;
	uint16_t		size;
	uint16_t		count;
}CB_t;

/**
* @brief - Adds an item to the circular buffer
* Takes the pointer to the circular buffer and the data item to be added
* Returns the status code depending on the success of the add operation
* @param - cbuffer CB_t *
* @param - dataToAdd bufferElement_t
* @return  CB_Status_t
**/
CB_Status_t CB_buffer_add_item(CB_t *cbuffer, bufferElement_t dataToAdd);

/**
* @brief - Removes an item from the circular buffer
* Takes the pointer to the circular buffer and the pointer to the variable to store the removed data item
* Returns the status code depending on the success of the removal operation
* @param - cbuffer CB_t *
* @param - dataToAdd bufferElement_t
* @return  CB_Status_t
**/
CB_Status_t CB_buffer_remove_item(CB_t *cbuffer, bufferElement_t *outData);

/**
* @brief - Checks if the circular buffer is full
* Takes the pointer to the circular buffer which is to be checked
* Returns the status code depending on whether the buffer is full
* @param - cbuffer CB_t *
* @return  CB_Status_t
**/
CB_Status_t CB_is_full(CB_t *cbuffer);

/**
* @brief - Checks if the circular buffer is empty
* Takes the pointer to the circular buffer which is to be checked
* Returns the status code depending on whether the buffer is empty
* @param - cbuffer CB_t *
* @return  CB_Status_t
**/
CB_Status_t CB_is_empty(CB_t *cbuffer);

/**
* @brief - Peeks into the buffer to a particular no. of positions from the head
* Takes the pointer to the circular buffer and the position from the head of the buffer to peek into
* Returns the status code depending on whether the peek operation was successful
* @param - cbuffer CB_t *
* @param - position uint16_t
* @param - outPeekData bufferElement_t *
* @return  CB_Status_t
**/
CB_Status_t CB_peek(CB_t *cbuffer, uint16_t position, bufferElement_t *outPeekData);

/**
* @brief - Allocates memory for the buffer and initializes the size, count, head and tail for the buffer
* Takes the pointer to the circular buffer and the length to be reserved for the buffer
* Returns the status code depending on whether the buffer initialization is successful
* @param - cbuffer CB_t *
* @param - length uint16_t
* @return  CB_Status_t
**/
CB_Status_t CB_init(CB_t *cbuffer, uint16_t length);

/**
* @brief - Deallocated the memory reserved for the buffer
* Takes the pointer to the circular buffer which is to be destroyed
* Returns the status code depending on whether the deallocation operation was successful
* @param - cbuffer CB_t *
* @return  CB_Status_t
**/
CB_Status_t CB_destroy(CB_t *cbuffer);

/**
* @brief - Helper function for printing the error string 
* Takes the status code enum
* Returns a pointer to one of the strings depending on the enum value
* @param - cbStatusEnum CB_Status_t
* @return  const uint8_t *
**/
const uint8_t* get_CB_error_String(CB_Status_t cbStatusEnum);

#endif
