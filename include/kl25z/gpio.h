/**
* @file - gpio.h
* @brief - Gives the HAL for GPIO ports/pins
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 27/10/2017
**/

#ifndef __GPIO_H__
#define __GPIO_H__

#include "MKL25Z4.h"
#include <stdint.h>

/**
* @brief - Enum for GPIO direction values
*/
typedef enum
{
	gpio_input	= 0,
	gpio_output = 1
}GPIO_PORT_DIR_t;

/**
* @brief - Enum for GPIO ports
*/
typedef enum
{
	gpioPortA = 0,
	gpioPortB,
	gpioPortC,
	gpioPortD,
	gpioPortE
}GPIO_PORT_t;

/**
* @brief - Enum for GPIO alternate function values
*/
typedef enum
{
	gpioAlt0_Disabled = 0,
	gpioAlt1_GPIO,
	gpioAlt2,
	gpioAlt3,
	gpioAlt4,
	gpioAlt5,
	gpioAlt6,
	gpioAlt7
}GPIO_ALT_FUNC_t;

typedef enum
{
	interrupt_disabled = 0,
	dma_req_rising_edge = 1,
	interrupt_falling_edge = 10
}GPIO_Interrup_Type_t;

/**
* @brief - Enables the GPIO PORT A
**/
void GPIO_PORTA_ENABLE();

/**
* @brief - Enables the GPIO PORT B
**/
void GPIO_PORTB_ENABLE();

/**
* @brief - Enables the GPIO PORT C
**/
void GPIO_PORTC_ENABLE();

/**
* @brief - Enables the GPIO PORT D
**/
void GPIO_PORTD_ENABLE();

/**
* @brief - Enables the GPIO PORT E
**/
void GPIO_PORTE_ENABLE();

/**
* @brief 	Enables the GPIO PORT
* @param 	GPIO_PORT_t gpioPort
* @return	void
**/
void GPIO_PORT_ENABLE(GPIO_PORT_t gpio);

/**
* @brief 	Sets the given pin of given port as input or output
* @param 	GPIO_PORT_t gpioPort
* @param	uint8_t pinNumber
* @param	GPIO_PORT_DIR_t direction
* @return	void
**/
void GPIO_PinDir(GPIO_PORT_t gpioPort, uint8_t pin, GPIO_PORT_DIR_t dir);

/**
* @brief 	Clears(writes 0) the given pin of given port
* @param 	GPIO_PORT_t gpioPort
* @param	uint8_t pinNumber
* @return	void
**/
void GPIO_PinOutClear(GPIO_PORT_t gpioPort, uint8_t pin);

/**
* @brief 	Sets(writes 1) the given pin of given port
* @param 	GPIO_PORT_t gpioPort
* @param	uint8_t pinNumber
* @return	void
**/
void GPIO_PinOutSet(GPIO_PORT_t gpioPort, uint8_t pin);

/**
* @brief 	Toggles the given pin of given port
* @param 	GPIO_PORT_t gpioPort
* @param	uint8_t pinNumber
* @return	void
**/
void GPIO_PinOutToggle(GPIO_PORT_t gpioPort, uint8_t pin);
/**
* @brief 	Gets the state(1/0) of the given output pin of given port
* @param 	GPIO_PORT_t gpioPort
* @param	uint8_t pinNumber
* @return	uint8_t state
**/
uint8_t GPIO_PinOutGet(GPIO_PORT_t gpioPort, uint8_t pin);

/**
* @brief 	Gets the state(1/0) of the given input pin of given port
* @param 	GPIO_PORT_t gpioPort
* @param	uint8_t pinNumber
* @return	uint8_t state
**/
uint8_t GPIO_PinInGet(GPIO_PORT_t gpioPort, uint8_t pin);

/**
* @brief 	Configures the given pin of given port to a particular alternate function
* @param 	GPIO_PORT_t gpioPort
* @param	uint8_t pinNumber
* @param	GPIO_ALT_FUNC_t alternate_function_selection
* @return	void
**/
void GPIO_PinAltFuncSel(GPIO_PORT_t gpioPort, uint8_t pin, GPIO_ALT_FUNC_t altFunctionSel);

/**
* @brief - Sets the RED LED on KL25Z
**/
void GPIO_Red_Led_En();

/**
* @brief - Sets the GREEN LED on KL25Z
**/
void GPIO_Green_Led_En();

/**
* @brief - Sets the BLUE LED on KL25Z
**/
void GPIO_Blue_Led_En();

/**
* @brief - Turns on the RED LED on KL25Z
**/
void GPIO_Red_On();

/**
* @brief - Turns off the RED LED on KL25Z
**/
void GPIO_Red_Off();

/**
* @brief - Toggles the RED LED on KL25Z
**/
void GPIO_Red_Toggle();

/**
* @brief - Turns on the GREEN LED on KL25Z
**/
void GPIO_Green_On();

/**
* @brief - Turns off the GREEN LED on KL25Z
**/
void GPIO_Green_Off();

/**
* @brief - Toggles the Green LED on KL25Z
**/
void GPIO_Green_Toggle();

/**
* @brief - Turns on the BLUE LED on KL25Z
**/
void GPIO_Blue_On();

/**
* @brief - Turns off the BLUE LED on KL25Z
**/
void GPIO_Blue_Off();

/**
* @brief - Toggles the Blue LED on KL25Z
**/
void GPIO_Blue_Toggle();

/**
* @brief - Configures the interrupt on GPIO pin
**/
void GPIO_config_interrupt(GPIO_PORT_t gpioPort, uint8_t pin, GPIO_Interrup_Type_t interrupt_type);

/**
* @brief - Gets the interrupt status of GPIO pin
**/
uint8_t GPIO_interrupt_status(GPIO_PORT_t gpioPort, uint8_t pin);

/**
* @brief - Clears the interrupt status on GPIO pin
**/
void GPIO_clear_interrupt(GPIO_PORT_t gpioPort, uint8_t pin);

#endif /* __GPIO_H__ */
