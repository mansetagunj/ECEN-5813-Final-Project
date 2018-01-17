/**
* @file - logger_queue.h
* @brief - Gives the logger queue data structure to hold the log structure
*
* @author Gunj/Ashish University of Colorado Boulder
* @date
**/
//
//#ifndef __LOGGER_QUEUE_H__
//#define __LOGGER_QUEUE_H__
//
//#include <stdint.h>
//#include "logger.h"
//
//typedef log_t loggerQElement_t;
//typedef log_t* p_loggerQElement_t;
//
//typedef enum loggerQ_status{
//	LOGGERQ_SUCCESS,
//	LOGGERQ_BUFFER_EMPTY,
//	LOGGERQ_BUFFER_NOT_EMPTY,
//	LOGGERQ_BUFFER_FULL,
//	LOGGERQ_BUFFER_NOT_FULL,
//	LOGGERQ_NULL_POINTER_ERROR,
//	LOGGERQ_BUFFER_NOT_ALLOCATED,
//	LOGGERQ_BUFFER_ALLOCATION_FAILURE
//}LOGGERQ_Status_t;
//
//
//typedef struct Logger_Queue{
//	loggerQElement_t *buffer;
//	loggerQElement_t *head;
//	loggerQElement_t *tail;
//	uint16_t		size;
//	uint16_t		count;
//}Logger_Queue_t;
//
///**
//* @brief - Adds an item to the circular buffer
//* Takes the pointer to the circular buffer and the data item to be added
//* Returns the status code depending on the success of the add operation
//* @param - loggerQ Logger_Queue_t *
//* @param - dataToAdd loggerQElement_t
//* @return  LOGGERQ_Status_t
//**/
//LOGGERQ_Status_t LOGGERQ_buffer_add_item(Logger_Queue_t *loggerQ, loggerQElement_t dataToAdd);
//
///**
//* @brief - Removes an item from the circular buffer
//* Takes the pointer to the circular buffer and the pointer to the variable to store the removed data item
//* Returns the status code depending on the success of the removal operation
//* @param - loggerQ Logger_Queue_t *
//* @param - dataToAdd loggerQElement_t
//* @return  LOGGERQ_Status_t
//**/
//LOGGERQ_Status_t LOGGERQ_buffer_remove_item(Logger_Queue_t *loggerQ, loggerQElement_t *outData);
//
///**
//* @brief - Checks if the circular buffer is full
//* Takes the pointer to the circular buffer which is to be checked
//* Returns the status code depending on whether the buffer is full
//* @param - loggerQ Logger_Queue_t *
//* @return  LOGGERQ_Status_t
//**/
//LOGGERQ_Status_t LOGGERQ_is_full(Logger_Queue_t *loggerQ);
//
///**
//* @brief - Checks if the circular buffer is empty
//* Takes the pointer to the circular buffer which is to be checked
//* Returns the status code depending on whether the buffer is empty
//* @param - loggerQ Logger_Queue_t *
//* @return  LOGGERQ_Status_t
//**/
//LOGGERQ_Status_t LOGGERQ_is_empty(Logger_Queue_t *loggerQ);
//
///**
//* @brief - Peeks into the buffer to a particular no. of positions from the head
//* Takes the pointer to the circular buffer and the position from the head of the buffer to peek into
//* Returns the status code depending on whether the peek operation was successful
//* @param - loggerQ Logger_Queue_t *
//* @param - position uint16_t
//* @param - outPeekData loggerQElement_t *
//* @return  LOGGERQ_Status_t
//**/
//LOGGERQ_Status_t LOGGERQ_peek(Logger_Queue_t *loggerQ, uint16_t position, loggerQElement_t *outPeekData);
//
///**
//* @brief - Allocates memory for the buffer and initializes the size, count, head and tail for the buffer
//* Takes the pointer to the circular buffer and the length to be reserved for the buffer
//* Returns the status code depending on whether the buffer initialization is successful
//* @param - loggerQ Logger_Queue_t *
//* @param - length uint16_t
//* @return  LOGGERQ_Status_t
//**/
//LOGGERQ_Status_t LOGGERQ_init(Logger_Queue_t *loggerQ, uint16_t length);
//
///**
//* @brief - Deallocated the memory reserved for the buffer
//* Takes the pointer to the circular buffer which is to be destroyed
//* Returns the status code depending on whether the deallocation operation was successful
//* @param - loggerQ Logger_Queue_t *
//* @return  LOGGERQ_Status_t
//**/
//LOGGERQ_Status_t LOGGERQ_destroy(Logger_Queue_t *loggerQ);
//
///**
//* @brief - Helper function for printing the error string
//* Takes the status code enum
//* Returns a pointer to one of the strings depending on the enum value
//* @param - cbStatusEnum LOGGERQ_Status_t
//* @return  const uint8_t *
//**/
//const uint8_t* get_LOGGERQ_error_String(LOGGERQ_Status_t loggerQStatusEnum);
//
//
//
//#endif /* __LOGGER_QUEUE_H__ */
