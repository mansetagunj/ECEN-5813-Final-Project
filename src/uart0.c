/**
* @file - uart0.c
* @brief - Implementation file for the UART0 functions
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 27/10/2017
**/

#include "MKL25Z4.h"
#include "mcg.h"
#include "uart0.h"
#include "gpio.h"
#include <malloc.h>
#include "conversion.h"
#include "memory.h"
#include "stdarg.h"
#include "stdio.h"
#include "platform.h"
#include "logger.h"

#define UART0ODE			(0)	//Open drain disable
#define UART0RXSRC			(0)	//UART0_Rx pin
#define UART0TXSRC			(0)	//UART0_Tx pin
#define UART0_CLKSEL_FLL	(0) //Select FLL as UART0 clk to MCGCLKFLL
#define UART0CLKSRC_FLLPLL	(1)	//MCGFLLCLK or MCGPLLCLK/2
#define UART0CLK_GATE_EN	(1)	//UART0 clock gate enable

#define _OSR_16_REG 		(15)
#define OSR_16 				(16)
#define _OSR_32_REG 		(31)
#define OSR_32				(32)
#define _OSR				(OSR_16)
#define _OSR_REG			(_OSR_16_REG)

#define BUFFER_TX_LEN		(4096)
#define BUFFER_RX_LEN		(64)

const uint16_t BUFFER_COUNT_THRESHOLD = ((BUFFER_TX_LEN*2)/3);

CB_t *UART0_TX_buffer = NULL;
CB_t *UART0_RX_buffer = NULL;

volatile uint8_t RX_bufferDataCount = 0;
volatile uint8_t processDataNow =0 ;
uint8_t bufferSet = 0;

void UART0_getBuffer(CB_t *outTXBuffer, CB_t *outRXBuffer)
{
	if(bufferSet)
	{
		outTXBuffer = UART0_TX_buffer;
		outRXBuffer = UART0_RX_buffer;
	}
}

CB_Status_t UART0_setBuffer(CB_t *TXBuffer, CB_t *RXBuffer)
{
	if(bufferSet)
		return CB_SUCCESS;

	if(NULL == TXBuffer)
	{
		UART0_TX_buffer = (CB_t*)malloc(sizeof(CB_t));
		if(NULL == UART0_TX_buffer)
			return CB_BUFFER_ALLOCATION_FAILURE;
		if(CB_SUCCESS == CB_init(UART0_TX_buffer,BUFFER_TX_LEN))
			TXBuffer = UART0_TX_buffer;
		else
			return CB_BUFFER_ALLOCATION_FAILURE;

	}
	else if(NULL == TXBuffer->buffer)
	{
		if(CB_BUFFER_ALLOCATION_FAILURE == CB_init(UART0_TX_buffer,BUFFER_TX_LEN))
			return CB_BUFFER_ALLOCATION_FAILURE;
		TXBuffer = UART0_TX_buffer;
	}
	else
		UART0_TX_buffer = TXBuffer;


	if(NULL == RXBuffer)
	{
		UART0_RX_buffer = (CB_t*)malloc(sizeof(CB_t));
		if(NULL == UART0_RX_buffer)
		{
			free(UART0_TX_buffer->buffer);	//we need to free the UART0_TX_buffer->buffer allocated above.
			free((void*)UART0_TX_buffer);			//we also need to free the UART0_TX_buffer
			UART0_TX_buffer = NULL;
			TXBuffer = NULL;
			return CB_BUFFER_ALLOCATION_FAILURE;
		}
		if(CB_SUCCESS == CB_init(UART0_RX_buffer,BUFFER_RX_LEN))
			RXBuffer = UART0_RX_buffer;
		else
			return CB_BUFFER_ALLOCATION_FAILURE;
	}
	else if(NULL == RXBuffer->buffer)
	{
		if(CB_BUFFER_ALLOCATION_FAILURE == CB_init(UART0_RX_buffer,BUFFER_RX_LEN))
		{
			free(UART0_TX_buffer->buffer);	//we need to free the UART0_TX_buffer->buffer allocated above.
			free((void*)UART0_TX_buffer);			//we also need to free the UART0_TX_buffer
			UART0_TX_buffer = NULL;
			TXBuffer = NULL;
			return CB_BUFFER_ALLOCATION_FAILURE;
		}
		RXBuffer = UART0_RX_buffer;
	}
	else
		UART0_RX_buffer = RXBuffer;

	bufferSet = 1;
	return CB_SUCCESS;
}

int8_t UART0_configure(BAUD_RATE_ENUM baudRateSel)
{
	if(baudRateSel != BAUD_115200 && baudRateSel != BAUD_38400 && baudRateSel != BAUD_57200 && baudRateSel != BAUD_9600)
		return -1;

	//selecting the FLL clock source for UART0
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(UART0CLKSRC_FLLPLL);
	SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(UART0_CLKSEL_FLL);

	//UART0 clock gate enable
	SIM->SCGC4 |= SIM_SCGC4_UART0(UART0CLK_GATE_EN);

	//Selecting the UART0 RX/TX pin behavior and source
	SIM->SOPT5 |= SIM_SOPT5_UART0ODE(UART0ODE);
	SIM->SOPT5 |= SIM_SOPT5_UART0RXSRC(UART0RXSRC);
	SIM->SOPT5 |= SIM_SOPT5_UART0TXSRC(UART0TXSRC);

	//Disabling the RX TX before configuring UART0
	UART0->C2 = 0;

	//Selecting 8 bit data, 1 stop bit, No parity
	UART0->C1 |= UART_C1_M(0) | UART_C1_PE(0);
	UART0->BDH |= UART_BDH_SBNS(0);

	//Setting OSR bits to 0b01111 = 15, which gives OSR to 16
	UART0->C4 |= UART0_C4_OSR(_OSR_REG);

	//SBR(BR) 1-8191 i.e. 13 bit value
	//Calculating the correct SBR(BR) for the selected BAUDRATE keeping Clock of 48MHz, and OSR of 15+1
	//Formula used to calculate -> BaudRate = BaudClock/((OSR+1)*BR)
	//where BaudClock is the clock freq used for UART, BR=SBR
	uint16_t SBR = (uint16_t)(((uint32_t)FLL_CLK/(baudRateSel*_OSR)) & 0x1FFF);
	UART0->BDL = UART_BDL_SBR(SBR);
	UART0->BDH |= UART_BDH_SBR(SBR>>8);

	//Enabling RIE Interrupt and the TCIE interrupt now.
	UART0->C2 |= UART_C2_RIE(1) | UART_C2_TCIE(1);

	//Enabling Rx and TX
	UART0->C2 |= UART_C2_RE(1) | UART_C2_TE(1);

	//Allocates TX/RX buffer. If fails, gives allocation failure.
	if(CB_BUFFER_ALLOCATION_FAILURE == UART0_setBuffer(NULL,NULL))
		return -2;

	//Enabling the NVIC Interrupt for UART0
	NVIC_EnableIRQ(UART0_IRQn);

	//Enabling the portA as the UART0 TX/RX pins are on portA
	GPIO_PORTA_ENABLE();
	//Setting the GPIO PA1 and PA2 to alt function 2 for UART0 Rx/Tx
	GPIO_PinAltFuncSel(gpioPortA, 1, gpioAlt2);
	GPIO_PinAltFuncSel(gpioPortA, 2, gpioAlt2);

//	GPIO_Green_Led_En();
//	GPIO_Red_Led_En();

	return 0;

}


void UART0_send(uint8_t *sendData)
{
	__disable_irq();
	if(NULL != sendData)
	{
		while(!(UART0->S1 & UART_S1_TDRE_MASK)); //Waiting for the buffer to get empty
		UART0->D = *sendData;
		while(!(UART0->S1 & UART_S1_TC_MASK)); //Waiting for transmission to get complete
	}
	__enable_irq();

}

void UART0_sendN(uint8_t *sendDataN, size_t len)
{
	__disable_irq();
	if(NULL != sendDataN)
	{
		size_t i = 0;
		while(i < len)
		{
			UART0_send(sendDataN + i);
			i++;
		}
	}
	__enable_irq();
}

void UART0_puts(uint8_t *sendDataN)
{
	if(NULL != sendDataN)
	{
		size_t i = 0;
		while(*(sendDataN+i))
		{
			UART0_CBsend(sendDataN+i);
			i++;
		}
	}
}

void UART0_putstr(const char *sendDataN)
{
	if(NULL != sendDataN)
	{
		size_t i = 0;
		while(*(sendDataN+i))
		{
			if(UART0_TX_buffer->count < BUFFER_COUNT_THRESHOLD)
			{
				UART0_CBsend((uint8_t*)sendDataN+i);
				i++;
			}
			else
				for(uint8_t a = 0; a < 200; a++)	//allowing the CB buffer to get empty
				{
					int b = 0;
					b++;
				}

		}

	}
}

char* convert(unsigned int num, int base)
{
	static char buf[33];
	char *ptr;

	ptr=&buf[sizeof(buf)-1];
	*ptr='\0';
	do
	{
	*--ptr="0123456789abcdef"[num%base];
	num/=base;
	}while(num!=0);
	return(ptr);
}

void UART0_printf(char *fmt, ...)
{
	char *p;
	int i;
	unsigned int u;
	char *s;
	double d;
	char str[6];
	va_list argp;

	va_start(argp, fmt);

	p=fmt;
	for(p=fmt; *p!='\0';p++)
	{
		if(*p != '%')
		{
			UART0_CBsend((uint8_t*)p);
			continue;
		}

		p++;

		switch(*p)
		{
		case 'f' :
			d=va_arg(argp,double);
			if(d<0)
			{
				d=-d;
				UART0_CBsend((uint8_t*)'-');;
			}
			sprintf(str,"%f",d);
			UART0_putstr(str);
			break;
		case 'c' :
			i=va_arg(argp,int);
			UART0_CBsend((uint8_t*)&i);
			break;
		case 'd' :
			i=va_arg(argp,int);
			if(i<0)
			{
				i=-i;
				UART0_CBsend((uint8_t*)'-');
			}
			UART0_putstr(convert(i,10));
			break;
		case 'o':
			i=va_arg(argp,unsigned int);
			UART0_putstr(convert(i,8));
			break;
		case 's':
			s=va_arg(argp,char *);
			UART0_putstr(s);
			break;
		case 'u':
			u=va_arg(argp,unsigned int);
			UART0_putstr(convert(u,10));
			break;
		case 'x':
			u=va_arg(argp,unsigned int);
			UART0_putstr(convert(u,16));
			break;
		case '%':
			UART0_CBsend((uint8_t*)'%');
			break;
		}
	}

	va_end(argp);
}



void UART0_CBsend(uint8_t *sendData)
{
//	__disable_irq();
	//if(CB_BUFFER_NOT_FULL == CB_is_full(UART0_TX_buffer))
	//{
//	CB_Status_t status = 1;
	__disable_irq();
	CB_buffer_add_item(UART0_TX_buffer,*sendData);
	UART0_TX_INT_ENABLE;
	__enable_irq();
	//}
	//else	//TX buffer is full.
	//{
	//}
//	UART0_TX_INT_ENABLE;
//	__enable_irq();
}

void UART0_receive(uint8_t *recvData)
{
	__disable_irq();
	if(NULL != recvData)
	{
		while((UART0->S1 & UART_S1_RDRF_MASK) == 0); //Waiting for the data to recv
		*recvData = UART0->D;
	}
	__enable_irq();
}

void UART0_receiveN(uint8_t *recvDataN, size_t len)
{
//	__disable_irq();
	if(NULL != recvDataN)
	{
		size_t i = 0;
		while(i < len)
		{
			__disable_irq();
			UART0_receive(recvDataN+i);
			i++;
			__enable_irq();
		}
	}
//	__enable_irq();
}


void UART0_CBsendN(uint8_t *sendDataN, size_t len)
{

	size_t itr = 0;
	CB_Status_t status = CB_SUCCESS;
	while((CB_SUCCESS == status ) && (itr < len))
	{
		__disable_irq();
		status = CB_buffer_add_item(UART0_TX_buffer,*(sendDataN+itr));
		itr++;
		UART0_TX_INT_ENABLE;
		__enable_irq();
	}
}

void UART0_CBreceive(uint8_t *recvData)
{
//	__disable_irq();
//	if(CB_BUFFER_NOT_EMPTY == CB_is_empty(UART0_RX_buffer))
//	{
		__disable_irq();
		CB_Status_t status = CB_buffer_remove_item(UART0_RX_buffer,recvData);
		if(status == CB_SUCCESS)
			UART0_RX_INT_ENABLE;
		else
			*recvData = 0xFF;
		__enable_irq();
//	}
//	else //RX Buffer is empty.
//	{
//		*recvData = 0xFF;
//	}
//	__enable_irq();
}

void UART0_CBreceiveN(uint8_t *recvDataN, size_t len)
{
	size_t itr = 0;
	CB_Status_t status = CB_SUCCESS;
	while((CB_SUCCESS == status) && (itr < len))
	{
		__disable_irq();
		status = CB_buffer_remove_item(UART0_RX_buffer,(recvDataN+itr));
		itr++;
		UART0_RX_INT_ENABLE;
		__enable_irq();
	}
}

void UART0_logFlush()
{
	while(UART0_TX_buffer->count != 0);
}


void UART0_IRQHandler(void)
{
	__disable_irq();

    unsigned char UART0_bufferData;

    if((UART0->S1 & UART_S1_TC_MASK) && (UART0->C2 & UART_C2_TCIE_MASK))	//TX TC flag is set and We are ready to transfer data
    {
//    	GPIO_Green_On();

    	UART0->S1 &= ~UART_S1_TC_MASK;
    	CB_Status_t status = CB_buffer_remove_item(UART0_TX_buffer,&UART0_bufferData);
    	if(CB_SUCCESS == status)
    	{
    		UART0->D = UART0_bufferData;
    		UART0_TX_INT_ENABLE;
    	}
    	else if(CB_BUFFER_EMPTY == status)	//TX buffer empty
    	{
    		UART0_TX_INT_DISABLE;	//We dont have anything to transmit, so disable the TX Int. Tx int will be enabled by UART_Send functions.
    	}
    	else	//Some other Buffer error conditions
    	{
    	}

		//GPIO_Green_Off();
    }
    else if((UART0->S1 & UART_S1_RDRF_MASK) && (UART0->C2 & UART_C2_RIE_MASK))
	{
		//GPIO_Red_On();

		UART0_bufferData = UART0->D;
		CB_Status_t status = CB_buffer_add_item(UART0_RX_buffer, UART0_bufferData);
		if(CB_SUCCESS == status)
		{
			logger_log(DATA_RECEIVED,"Data: %c",UART0_bufferData);
			(UART0_bufferData == '~') ? logging = 0 : 0;
			(UART0_bufferData == '!') ? logging = 1 : 0;
			(UART0_bufferData == '|') ? log_format = BINARY_LOGGER : 0;
			(UART0_bufferData == '/') ? log_format = ASCII_LOGGER : 0;
			(UART0_bufferData == '\r') ? processDataNow = 1: 0;
		}
		else if(CB_BUFFER_FULL == status)	//RX Buffer is full
		{
			UART0_RX_INT_DISABLE;	//We can't handle more data, so disable the RX Int. Rx int will be enabled by UART_Recv functions which will remove some items from the buffer
			//UART0_putstr("\r\nRX BUFFER IS FULL");
		}
		//GPIO_Red_Off();
	}
	__enable_irq();
}

