/**
* @file - spi.h
* @brief - Header file for the library functions for SPI
*
* @author Gunj/Ashish University of Colorado Boulder
* @date - 8 Dec 2017
**/

#ifndef _SPI_H_
#define _SPI_H_

#include "MKL25Z4.h"
#include "mcg.h"
#include "gpio.h"
#include <stddef.h>

/**
* @brief - Enum to allow flexibility of selection between SPI0 and SPI1
**/
typedef enum{
	SPI_0,
	SPI_1
}SPI_t;

SPI_Type *SPI[2];

/**
* @brief - Initialize the GPIO pins associated with SPI
* Configure SPI in 3 wire mode and use a GPIO pin for chip select
* @param - spi SPI_t
* @return void
**/
void SPI_GPIO_init(SPI_t spi);

/**
* @brief - Enable the clock gate control for SPI
* @param - spi SPI_t
* @return void
**/
void SPI_clock_init(SPI_t spi);

/**
* @brief - Perform the initialization routine for the SPI module
* @param - spi SPI_t
* @return void
**/
void SPI_init(SPI_t spi);

/**
* @brief - Disable the GPIO pins earlier initialized for the SPI module
* @return void
**/
void SPI_disable();

/**
* @brief - Read a single byte from the SPI bus
* @param - spi SPI_t
* @return uint8_t
**/
uint8_t SPI_read_byte(SPI_t spi);

/**
* @brief - Write a single byte on to the SPI bus
* @param - spi SPI_t
* @param - byte uint8_t
* @return void
**/
void SPI_write_byte(SPI_t spi, uint8_t byte);

/**
* @brief - Send a packet on to the SPI bus
* Send multiple bytes given a pointer to an array and the number of bytes to be sent
* @param - spi SPI_t
* @param - p uint8_t
* @param - length size_t
* @return void
**/
void SPI_write_packet(SPI_t spi, uint8_t* p, size_t length);

/**
* @brief - Read a packet from the SPI bus
* Read multiple bytes given a pointer to an array for storage and the number of bytes to be read
* @param - spi SPI_t
* @param - p uint8_t *
* @param - length size_t
* @return void
**/
void SPI_read_packet(SPI_t spi, uint8_t* p, size_t length);

/**
* @brief - Blocks until SPI transmit buffer has completed transmitting
* @param - spi SPI_t
* @return void
**/
__STATIC_INLINE void SPI_flush(SPI_t spi) {
	while ((SPI[spi]->S & SPI_S_SPTEF_MASK) == 0);
}

#endif /* SOURCES_SPI0_H_ */
