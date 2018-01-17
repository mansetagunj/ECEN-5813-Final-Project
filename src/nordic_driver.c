/**
* @file - nordic_driver.c
* @brief - Implementation file for the driver functions of the NRF240L
*
* @author Gunj/Ashish University of Colorado Boulder
* @date - 8 Dec 2017
**/


#include <stdint.h>

#include "nordic_driver.h"
#include "logger.h"
#include "spi.h"
#include "gpio.h"

//Commands Byte
#define NORDIC_TXFIFO_FLUSH_CMD	(0xE1)
#define NORDIC_RXFIFO_FLUSH_CMD	(0xE2)
#define NORDIC_W_TXPAYLD_CMD	(0xA0)
#define NORDIC_R_RXPAYLD_CMD	(0x61)
#define NORDIC_ACTIVATE_CMD		(0x50)
#define NORDIC_ACTIVATE_DATA	(0x73)
#define NORDIC_RXPAYLD_W_CMD	(0x60)
#define NORDIC_NOP				(0xFF)
//Register Addresses
#define NORDIC_CONFIG_REG		(0x00)
#define NORDIC_STATUS_REG 		(0x07)
#define NORDIC_RF_SETUP_REG		(0x06)
#define NORDIC_RF_CH_REG		(0x05)
#define NORDIC_TX_ADDR_REG		(0x10)
#define NORDIC_TX_ADDR_LEN		(5)
#define NORDIC_FIFO_STATUS_REG	(0x17)
#define NORDIC_RX_PW_P0_REG		(0x11)

#define DEFAULT_TX_ADDRESS_1B	(0xE7)
#define DEFAULT_TX_ADDRESS_2B	(0xE7)
#define DEFAULT_TX_ADDRESS_3B	(0xE7)
#define DEFAULT_TX_ADDRESS_4B	(0xE7)
#define DEFAULT_TX_ADDRESS_5B	(0xE7)

//Masks
#define NORDIC_CONFIG_RX_DR_MASK		6
#define NORDIC_CONFIG_RX_DR_INT(x)		((((uint8_t)x)<<NORDIC_CONFIG_RX_DR_MASK)&(1<<NORDIC_CONFIG_RX_DR_MASK))

#define NORDIC_CONFIG_TX_DS_MASK		5
#define NORDIC_CONFIG_TX_DS_INT(x)		((((uint8_t)x)<<NORDIC_CONFIG_TX_DS_MASK)&(1<<NORDIC_CONFIG_TX_DS_MASK))

#define NORDIC_CONFIG_PWR_UP_MASK		1
#define NORDIC_CONFIG_PWR_UP(x)			((((uint8_t)x)<<NORDIC_CONFIG_PWR_UP_MASK)&(1<<NORDIC_CONFIG_PWR_UP_MASK))

#define NORDIC_CONFIG_PRIM_RX_MASK		0
#define NORDIC_CONFIG_PRIM_RX(x)		((((uint8_t)x)<<NORDIC_CONFIG_PRIM_RX_MASK)&(1<<NORDIC_CONFIG_PRIM_RX_MASK))


#define NORDIC_STATUS_TX_FULL_MASK			(1<<0)
#define NORDIC_FIFO_STATUS_TX_FULL_MASK		(1<<5)
#define NORDIC_FIFO_STATUS_RX_FULL_MASK		(1<<1)
#define NORDIC_FIFO_STATUS_TX_EMPTY_MASK	(1<<4)
#define NORDIC_FIFO_STATUS_RX_EMPTY_MASK	(0<<5)

volatile uint8_t txconfigured = 0;
volatile uint8_t rxconfigured = 0;

volatile uint8_t transmitted = 0;
volatile uint8_t received = 0;

void NRF_gpioInit()
{

	//Enabling the GPIO PTD5 for Nordic CE pin
	GPIO_PORT_ENABLE(NORDIC_CE_PORT);
	GPIO_PinDir(NORDIC_CE_PORT,NORDIC_CE_PIN,gpio_output);
	GPIO_PinAltFuncSel(NORDIC_CE_PORT,NORDIC_CE_PIN,gpioAlt1_GPIO);
	GPIO_PinOutClear(NORDIC_CE_PORT,NORDIC_CE_PIN);

	//Enabling the GPIO PTA13 for Nordic IRQ pin
	GPIO_PORT_ENABLE(NORDIC_IRQ_PORT);
	GPIO_PinDir(NORDIC_IRQ_PORT,NORDIC_IRQ_PIN,gpio_input);
	GPIO_PinAltFuncSel(NORDIC_IRQ_PORT,NORDIC_IRQ_PIN,gpioAlt1_GPIO);

}


void NRF_moduleInit()
{
	NRF_gpioInit();

	SPI_init(SPI_0);
}

void NRF_moduleDisable()
{
	GPIO_PinAltFuncSel(NORDIC_CE_PORT,NORDIC_CE_PIN,gpioAlt0_Disabled);
	GPIO_PinAltFuncSel(NORDIC_IRQ_PORT,NORDIC_IRQ_PIN,gpioAlt0_Disabled);
	SPI_disable();
}

uint8_t NRF_read_register(uint8_t regAdd)
{
	//SPI_clear_RXbuffer(SPI_0);	//used to clear the previously value in the RX FIFO
	uint8_t readValue = 0;

	//CSN High to low for new command
	NRF_chip_disable();
	NRF_chip_enable();

	SPI_write_byte(SPI_0,regAdd);
	SPI_read_byte(SPI_0);	//used to clear the previously value in the RX FIFO
	SPI_write_byte(SPI_0,0xFF);
	readValue = SPI_read_byte(SPI_0);

	//Marking the end of transaction by CSN high
	NRF_chip_disable();

	return readValue;
}

void NRF_write_command(uint8_t command)
{
	//CSN High to low for new command
	NRF_chip_disable();
	NRF_chip_enable();

	SPI_write_byte(SPI_0,command);
	//SPI_clear_RXbuffer(SPI_0);	//used to clear the previously value in the RX FIFO
	SPI_read_byte(SPI_0);

	//Marking the end of transaction by CSN high
	NRF_chip_disable();
}

void NRF_write_register(uint8_t regAdd, uint8_t value)
{
	//SPI_clear_RXbuffer(SPI_0);	//used to clear the previously value in the RX FIFO

	//CSN High to low for new command
	NRF_chip_disable();
	NRF_chip_enable();

	SPI_write_byte(SPI_0,regAdd | 0x20);
	SPI_read_byte(SPI_0);	//used to clear the previously value in the RX FIFO
	SPI_write_byte(SPI_0,value);
	SPI_read_byte(SPI_0);	//used to clear the previously value in the RX FIFO

	//Marking the end of transaction by CSN high
	NRF_chip_disable();
}

void NRF_write_status(uint8_t statusValue)
{
	NRF_write_register(NORDIC_STATUS_REG, statusValue);
}
uint8_t NRF_read_status()
{
	return  NRF_read_register(NORDIC_STATUS_REG);
}

void NRF_write_config(uint8_t configValue)
{
	NRF_write_register(NORDIC_CONFIG_REG, configValue);
}

uint8_t NRF_read_config()
{
	return NRF_read_register(NORDIC_CONFIG_REG);
}

uint8_t NRF_read_rf_setup()
{
	return NRF_read_register(NORDIC_RF_SETUP_REG);
}

void NRF_write_rf_setup(uint8_t rfSetupValue)
{
	NRF_write_register(NORDIC_RF_SETUP_REG, rfSetupValue);
}

uint8_t NRF_read_rf_ch()
{
	return NRF_read_register(NORDIC_RF_CH_REG);
}

void NRF_write_rf_ch(uint8_t channel)
{
	NRF_write_register(NORDIC_RF_CH_REG, channel);
}

void NRF_read_TX_ADDR(uint8_t *address)
{
	uint8_t i = 0;

	NRF_chip_disable();
	NRF_chip_enable();

	SPI_write_byte(SPI_0,NORDIC_TX_ADDR_REG);
	SPI_read_byte(SPI_0);	//used to clear the previously value in the RX FIFO
	//SPI_read_byte(SPI_0);	//used to clear the previously value in the RX FIFO
	while(i < NORDIC_TX_ADDR_LEN)
	{
		SPI_write_byte(SPI_0, 0xFF);	//Dummy to get the data
		*(address+i) = SPI_read_byte(SPI_0);
		i++;
	}

	NRF_chip_disable();
}

void NRF_write_TX_ADDR(uint8_t * tx_addr)
{
	NRF_chip_disable();
	NRF_chip_enable();

	SPI_write_byte(SPI_0,NORDIC_TX_ADDR_REG | 0x20);
	SPI_read_byte(SPI_0);	//used to clear the previously value in the RX FIFO
	SPI_write_packet(SPI_0,tx_addr,NORDIC_TX_ADDR_LEN);
	SPI_read_byte(SPI_0); //used to clear the previously value in the RX FIFO
	SPI_read_byte(SPI_0); //used to clear the previously value in the RX FIFO

	NRF_chip_disable();
}

uint8_t NRF_read_fifo_status()
{
	return NRF_read_register(NORDIC_FIFO_STATUS_REG);
}

void NRF_flush_tx_fifo()
{
	NRF_write_command(NORDIC_TXFIFO_FLUSH_CMD);
}

void NRF_flush_rx_fifo()
{
	NRF_write_command(NORDIC_RXFIFO_FLUSH_CMD);
}

void NRF_activate_cmd()
{
	NRF_write_register(NORDIC_ACTIVATE_CMD, NORDIC_ACTIVATE_DATA);
}

void NRF_mode_configure(NRF_Mode_t mode)
{
	if(mode < 2)
	{
		uint8_t configureRead = NRF_read_config();

		if(mode == NRF_Mode_TX)
		{
			txconfigured = 1;
			rxconfigured = 0;
			//configureRead |= NORDIC_CONFIG_TX_DS_INT(1);
		}
		else
		{
			txconfigured = 0 ;
			rxconfigured = 1;
			//configureRead |= NORDIC_CONFIG_RX_DR_INT(1);
			NRF_transmit_enable();

		}

		NRF_write_config(configureRead | NORDIC_CONFIG_PRIM_RX(mode) | NORDIC_CONFIG_PWR_UP(1));
		NRF_flush_rx_fifo();
		NRF_flush_tx_fifo();
		GPIO_PORTA_ENABLE();
		GPIO_config_interrupt(gpioPortA,13,interrupt_falling_edge);
		logger_log(INFO,"NORDIC Configured in %d mode",mode);

	}
	else
		logger_log(ERROR,"INVALID MODE");
}

void NRF_transmit_data(uint8_t *data, uint8_t len)
{
	if(txconfigured)
	{
		NRF_chip_disable();
		NRF_chip_enable();

		NRF_flush_tx_fifo();

		NRF_transmit_disable();

		SPI_write_byte(SPI_0,NORDIC_W_TXPAYLD_CMD);
		SPI_read_byte(SPI_0);	//used to clear the previously value in the RX FIFO

		SPI_write_packet(SPI_0,data, len);	//loading the FIFO with data  before enabling the CE pin
		logger_log(INFO,"Data written");
		NRF_transmit_enable();

		SPI_read_byte(SPI_0); //used to clear the previously value in the RX FIFO
		//SPI_read_byte(SPI_0); //used to clear the previously value in the RX FIFO

		while(transmitted == 0);	//wait till TX data is transmitted from FIFO
		transmitted = 0;
//		while(!(NORDIC_STATUS_TX_DS_MASK & NRF_read_status()));
		NRF_transmit_disable();
		NRF_chip_disable();
		logger_log(INFO,"Data Transmitted");

	}
	else
		logger_log(ERROR,"TX mode not configured");
}

void NRF_read_data(uint8_t *data, uint8_t len)
{
	if(rxconfigured)
	{
		while(received == 0);	//wait till RX data in FIFO
		received = 0;
//		while(!(NORDIC_STATUS_RX_DR_MASK & NRF_read_status()));
		logger_log(INFO,"Data received");
		NRF_chip_disable();
		NRF_chip_enable();

		NRF_transmit_enable();

		SPI_write_byte(SPI_0,NORDIC_R_RXPAYLD_CMD);
		SPI_read_byte(SPI_0);	//used to clear the previously value in the RX FIFO
		SPI_read_packet(SPI_0,data,len);
		SPI_read_byte(SPI_0); //used to clear the previously value in the RX FIFO
		SPI_read_byte(SPI_0); //used to clear the previously value in the RX FIFO

		NRF_transmit_disable();
		NRF_chip_disable();
		logger_log(INFO,"Data read");

	}
	else
		logger_log(ERROR,"RX mode not configured");
}

