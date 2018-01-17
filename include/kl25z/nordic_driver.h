/**
* @file - nordic_driver.h
* @brief - Header file for the driver functions of the NRF240L
*
* @author Gunj/Ashish University of Colorado Boulder
* @date - 8 Dec 2017
**/

#ifndef __NORDIC_DRIVER_H__
#define __NORDIC_DRIVER_H__

#include "gpio.h"

#define NORDIC_CE_PORT gpioPortD
#define NORDIC_CE_PIN 5

#define NORDIC_CSN_PORT gpioPortD
#define NORDIC_CSN_PIN 0

#define NORDIC_IRQ_PORT gpioPortA
#define NORDIC_IRQ_PIN 13

#define NORDIC_STATUS_RX_DR_MASK			(1<<6)
#define NORDIC_STATUS_TX_DS_MASK			(1<<5)

volatile uint8_t txconfigured;
volatile uint8_t rxconfigured;

typedef enum{

	NRF_Mode_TX = 0,
	NRF_Mode_RX = 1

}NRF_Mode_t;

volatile uint8_t transmitted;
volatile uint8_t received;

/**
* @brief - Enable the chip select connection to Nordic
* @return void
**/
__STATIC_INLINE void NRF_chip_enable()
{
	GPIO_PinOutClear(NORDIC_CSN_PORT,NORDIC_CSN_PIN);
}

/**
* @brief - Disable the chip select connection to Nordic
* @return void
**/
__STATIC_INLINE void NRF_chip_disable()
{
	GPIO_PinOutSet(NORDIC_CSN_PORT,NORDIC_CSN_PIN);
}

/**
* @brief - Enable Transmission from the Nordic module
* @return void
**/
__STATIC_INLINE void NRF_transmit_enable()
{
	GPIO_PinOutSet(NORDIC_CE_PORT,NORDIC_CE_PIN);
}

/**
* @brief - Disable Transmission from the Nordic module
* @return void
**/
__STATIC_INLINE void NRF_transmit_disable()
{
	GPIO_PinOutClear(NORDIC_CE_PORT,NORDIC_CE_PIN);
}

/**
* @brief - Initialize the NRF module
* Initialized the GPIO connections pertaining to the Nordic module
* @return void
**/
void NRF_moduleInit();

/**
* @brief - Disable the GPIO connections set up earlier for the Nordic module
* @return void
**/
void NRF_moduleDisable();

/**
* @brief - Read a register from the NRF module
* @param - regAdd uint8_t
* @return uint8_t
**/
uint8_t NRF_read_register(uint8_t regAdd);

/**
* @brief - Write to a register from the NRF module
* @param - regAdd uint8_t
* @param - value uint8_t
* @return void
**/
void NRF_write_register(uint8_t regAdd, uint8_t value);

/**
* @brief - Write to the NRF module's status register
* @param - statusValue uint8_t
* @return void
**/
void NRF_write_status(uint8_t statusValue);

/**
* @brief - Read the NRF module's status register
* @return uint8_t
**/
uint8_t NRF_read_status();

/**
* @brief - Write to the NRF module's config register
* @param - configValue uint8_t
* @return void
**/
void NRF_write_config(uint8_t configValue);

/**
* @brief - Read the NRF module's config register
* @return uint8_t
**/
uint8_t NRF_read_config();


/**
* @brief - Read the NRF module's RF setup register
* @return uint8_t
**/
uint8_t NRF_read_rf_setup();

/**
* @brief - Write to the NRF module's RF setup register
* @param - rfStatusValue uint8_t
* @return void
**/
void NRF_write_rf_setup(uint8_t rfSetupValue);

/**
* @brief - Read the NRF module's RF CH register
* @return uint8_t
**/
uint8_t NRF_read_rf_ch();

/**
* @brief - Write to the NRF module's RF CH register
* @param - channel uint8_t
* @return void
**/
void NRF_write_rf_ch(uint8_t channel);

/**
* @brief - Reads 5 bytes of the NRF module's TX ADDR register
* @param - address uint8_t *
* @return void
**/
void NRF_read_TX_ADDR(uint8_t * address);

/**
* @brief - Writes 5 bytes of the NRF module's TX ADDR register
* @param - tx_addr uint8_t *
* @return void
**/
void NRF_write_TX_ADDR(uint8_t * tx_addr);

/**
* @brief - Read the NRF module's FIFO status register
* @return address uint8_t
**/
uint8_t NRF_read_fifo_status();

/**
* @brief - Send the command FLUSH_TX to the NRF module
* @return void
**/
void NRF_flush_tx_fifo();

/**
* @brief - Send the command FLUSH_RX to the NRF module
* @return void
**/
void NRF_flush_rx_fifo();

/**
* @brief - Send the activation command to the NRF module
* Activates the features: R_RX_PL_WID, W_ACK_PAYLOAD, W_TX_PAYLOAD_NOACK
* @return void
**/
void NRF_activate_cmd();

void NRF_mode_configure(NRF_Mode_t mode);
void NRF_read_data(uint8_t *data, uint8_t len);
void NRF_transmit_data(uint8_t *data, uint8_t len);



#endif /* __NORDIC_DRIVER_H__ */
