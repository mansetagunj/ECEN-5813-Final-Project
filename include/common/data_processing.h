/**
* @file - data_processing.h
* @brief - Header file for the data processing implementation
*
* @author Gunj/Ashish University of Colorado Boulder
* @date
**/

#ifndef __DATA_PROCESSING_H__
#define __DATA_PROCESSING_H__

#include "circular_buffer.h"

/**
* @brief - Processing the data from the RX buffer and logs the result
* @param - CB_t* RXBuffer
* @return void
**/
void processData(CB_t *RXBuffer);


#endif /* __DATA_PROCESSING_H__ */
