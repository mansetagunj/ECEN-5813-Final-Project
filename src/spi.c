/*
 * spi.c
 *
 *  Created on: Dec 1, 2017
 *      Author: ashis
 */

#include "spi.h"
#include "uart0.h"

SPI_Type *SPI[2] = {SPI0, SPI1};

void SPI_GPIO_init(SPI_t spi) {
	if(spi==SPI_0) {
		GPIO_PORTD_ENABLE();
		//Set SCK, MOSI and MISO pins for SPI functionality
		GPIO_PinAltFuncSel(gpioPortD,1,gpioAlt2);
		GPIO_PinAltFuncSel(gpioPortD,2,gpioAlt2);
		GPIO_PinAltFuncSel(gpioPortD,3,gpioAlt2);
		//Set the pin for Chip Selection logic as GPIO
		GPIO_PinAltFuncSel(gpioPortD,0,gpioAlt1_GPIO);
		GPIO_PinDir(gpioPortD,0,gpio_output);
	}
}

void SPI_disable()
{

	GPIO_PinAltFuncSel(gpioPortD,1,gpioAlt1_GPIO);
	GPIO_PinOutClear(gpioPortD,1);
	GPIO_PinAltFuncSel(gpioPortD,1,gpioAlt0_Disabled);
	GPIO_PinAltFuncSel(gpioPortD,2,gpioAlt0_Disabled);
	GPIO_PinAltFuncSel(gpioPortD,3,gpioAlt0_Disabled);
	GPIO_PinAltFuncSel(gpioPortD,0,gpioAlt0_Disabled);

}

void SPI_clock_init(SPI_t spi) {
	if(spi==SPI_0)
		SIM->SCGC4|= SIM_SCGC4_SPI0(1);
	else if (spi==SPI_1)
		SIM->SCGC4|= SIM_SCGC4_SPI1(1);
}

void SPI_init(SPI_t spi) {

	SPI_clock_init(spi);

	SPI_GPIO_init(spi);

	//Enable SPI Interrupt and Transmit interrupt
	//SPI[spi]->C1|= SPI_C1_SPIE(1) | SPI_C1_SPTIE(1);

	//Configure the device as Master
	SPI[spi]->C1|= SPI_C1_MSTR(1);

	//Idle low and be active on the rising edge
	SPI[spi]->C1|= SPI_C1_CPOL(0);

	//Configure for MSB first
	SPI[spi]->C1|= SPI_C1_LSBFE(0);

	SPI[spi]->C1 &= ~(SPI_C1_CPHA(1));
	//Slave Select Output Enable
	//SPI[spi]->C1|= SPI_C1_SSOE(1);

	//Set the Baud Rate Prescalar as 1 and the Baud Rate Divisor as 4
	//For Bus clock at 20 MHz, the frequency would be 5MHz
	SPI[spi]->BR|= SPI_BR_SPPR(0) | SPI_BR_SPR(2);

	//Master mode-fault function enable: To make the SS pin act as Slave Select output
	//SPI[spi]->C2|= SPI_C2_MODFEN(1);

	//SPI System Enable
	SPI[spi]->C1|= SPI_C1_SPE(1);

}

uint8_t SPI_read_byte(SPI_t spi) {

	while ((SPI[spi]->S & SPI_S_SPRF_MASK) == 0);
	return SPI[spi]->D;
}

void SPI_write_byte(SPI_t spi, uint8_t byte) {

	SPI_flush(spi);
	SPI[spi]->D = byte;
	SPI_flush(spi);
}

void SPI_write_packet(SPI_t spi, uint8_t* p, size_t length) {
	uint8_t i=0;
	while (i<length) {
		SPI_write_byte(spi, *(p+i));
		++i;
	}
}

void SPI_read_packet(SPI_t spi, uint8_t* p, size_t length) {
	uint8_t i=0;
	while (i<length) {
		*(p+i) = SPI_read_byte(spi);
		++i;
	}
}

void SPI0_IRQHandler() {
//	if (SPI0_S & SPI_S_SPRF_MASK) {
//		recd= SPI0_D;
//	}
	__disable_irq();



	uint8_t c = 'I';
	UART0_send(&c);
	__enable_irq();
}
