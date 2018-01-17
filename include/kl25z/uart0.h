/**
* @file - uart0.h
* @brief - Header file for the UART functions
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 27/10/2017
**/

#ifndef __UART0_H__
#define __UART0_H__

#include <stdint.h>
#include "circular_buffer.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef size_t
typedef unsigned int size_t;
#endif

#define PROCESS_DATA_AT_COUNT (32)

#define UART0_RX_INT_ENABLE		UART0->C2 |= UART_C2_RIE(1)
#define UART0_RX_INT_DISABLE	UART0->C2 &= ~(UART_C2_RIE(1))
#define UART0_TX_INT_ENABLE		UART0->C2 |= UART_C2_TCIE(1)
#define UART0_TX_INT_DISABLE	UART0->C2 &= ~(UART_C2_TCIE(1))

CB_t *UART0_TX_buffer;
CB_t *UART0_RX_buffer;

volatile uint8_t processDataNow;
volatile uint8_t RX_bufferDataCount;

/**
* @brief - Available Baud rates for the UART
*/
typedef enum BAUD_RATE
{
	BAUD_115200 = 115200,
	BAUD_38400 	= 38400,
	BAUD_57200	= 57200,
	BAUD_9600	= 9600,

}BAUD_RATE_ENUM;

/**
* @brief - Gets the TX/RX circular buffer handle(pointer) associated with the UART0 peripheral
* @param - [in_out] CB_t* TXBuffer
* @param - [in_out] CB_t* RXBuffer
* @return void
**/
void UART0_getBuffer(CB_t *outTXBuffer, CB_t *outRXBuffer);

/**
* @brief	Sets the handle provided as the parameter to the TX/RX circular buffer handle(pointer) associated with the UART0 peripheral
* 			1. If the buffer pointer provided is NULL - It creates a new buffer with the given pointer and initializes the buffer.
* 			2. If the given buffer is not initialized, it initializes the buffer
* @param	[in_out] CB_t* TXBuffer
* @param	[in_out] CB_t* RXBuffer
* @return 	void
**/
CB_Status_t UART0_setBuffer(CB_t *TXBuffer, CB_t *RXBuffer);

/**
* @brief	Configures the UART0 with necessary configuration
* 			1. UART baud rate can be given as a parameter using the BAUD_RATE_ENUM.
* 			   If the given baud rate is not from the one specified by the BAUD_RATE_ENUM,
* 			   the function will return -1 and the UART0 will not be configured.
* 			2. Configures the UART0 to use FLL clock. Assumes the clock to be of 48MHz
*			3. Allocates the TX/RX buffer is not allocated before. Function returns -2 when buffer allocation fails
*			3. Enables the UART0 interrupts and other configurations required for UART0 functioning
* @param  	[in] BAUD_RATE_ENUM baudRateSel
* @return 	int8_t
**/
int8_t UART0_configure(BAUD_RATE_ENUM baudRateSel);

/**
* @brief	Sends the data given as parameter. Blocking till data sent
* @param	[in] uint8_t* sendData
* @return 	void
**/
void UART0_send(uint8_t *sendData);

/**
* @brief	Sends a chunk of data given as parameter with size
* @param	[in] uint8_t* sendDataN
* @param	size_t len
* @return 	void
**/
void UART0_sendN(uint8_t *sendDataN, size_t len);

/**
* @brief	Sends a string
* @param	[in] uint8_t* sendDataN
* @return 	void
**/
void UART0_puts(uint8_t *sendDataN);

/**
* @brief	Sends a string
* @param	[in] char* sendDataN
* @return 	void
**/
void UART0_putstr(const 	char *sendDataN);

/**
* @brief	Receives the data and fills the data into the given parameter. Blocking till data recieved
* @param	[out] uint8_t* recvData
* @return 	void
**/
void UART0_receive(uint8_t *recvData);

/**
* @brief	Receives a chunk of data given as parameter with size
* @param	[out] uint8_t* recvDataN
* @param	size_t len
* @return 	void
**/
void UART0_receiveN(uint8_t *recvDataN, size_t len);

/**
* @brief	Stores the data given as parameter into the TX circular buffer which is send over interrupts.
* 			The data is lost if the TX circular buffer is full.
* @param	[in] uint8_t* sendData
* @return 	void
**/
void UART0_CBsend(uint8_t *sendData);

/**
* @brief	Stores the series of data of length given as parameters into the TX circular buffer which is send over interrupts.
* 			The data is lost if the TX circular buffer is full.
* @param	[in] uint8_t* sendDataN
* @param	size_t len
**/
void UART0_CBsendN(uint8_t *sendDataN, size_t len);

/**
* @brief	Gets the data from the RX Circular buffer. The RX circular buffer gets data from UART during Interrupts
* 			The parameter variable remains untouched is the RX circular buffer is empty
* @param	[out] uint8_t* recvData
* @return 	void
**/
void UART0_CBreceive(uint8_t *recvData);

/**
* @brief	Gets the chunk of data of given len from the RX Circular buffer. The RX circular buffer gets data from UART during Interrupts
* 			The parameter variable remains untouched is the RX circular buffer is empty
* 			If length of receivedData variable > RX circular buffer data count
* 				The remaining data remains untouched
* @param	[out] uint8_t* recvDataN
* @param	size_t len
* @return 	void
**/
void UART0_CBreceiveN(uint8_t *recvDataN, size_t len);

/**
* @brief	Works similar to printf function but flushes the data to UART0
*
* @param	char* fmt
* @param	variable argument
* @return 	void
**/
void UART0_printf(char *fmt, ...);

/**
* @brief	Flush the UART transmit buffer
* @return 	void
**/
void UART0_logFlush();

#endif
