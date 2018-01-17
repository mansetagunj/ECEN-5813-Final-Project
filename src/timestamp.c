/*
 * timestamp.c
 *
 *  Created on: 05-Dec-2017
 *      Author: Gunj Manseta
 */

#include "timestamp.h"
#include "logger.h"
#include "time.h"

#ifdef PLATFORM_KL25Z

#include "MKL25Z4.h"
#include "gpio.h"

#define G_Current_Time	RTC_TSR

void rtc_init()
{
	__disable_irq();
	NVIC_DisableIRQ(RTC_IRQn);
	NVIC_DisableIRQ(RTC_Seconds_IRQn);

	SIM->SOPT1 &= ~(SIM_SOPT1_OSC32KSEL(3));
	SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(3);

	//Enable RTC Access control and interrupts
	SIM->SCGC6|= SIM_SCGC6_RTC(1);

	RTC_CR = RTC_CR_SWR_MASK;
	RTC_CR&= ~RTC_CR_SWR_MASK;

	//Clear RTC interrupts
	//RTC->IER = 0x00;

	//Remove locks on Control, Status and Lock register
	RTC->LR|=RTC_LR_LRL(1) | RTC_LR_CRL(1) | RTC_LR_SRL(1);

	//Enable writing to registers in non-supervisor mode
	RTC->CR|=RTC_CR_SUP(1);

	//32.768 kHz oscillator is enabled
	//RTC->CR &= ~RTC_CR_OSCE(1);
	//RTC->CR|=RTC_CR_OSCE(1);

	//Disable counter, load Seconds and Prescalar registers and enable the counter again
	RTC_SR &= ~RTC_SR_TCE(1);
	RTC_TSR= BUILD_EPOCH_TIME;
	RTC_TPR|= RTC_TPR_TPR(0x7BFF);
	RTC_SR |=RTC_SR_TCE(1);

	RTC->IER |= RTC_IER_TSIE(1) | RTC_IER_TOIE(1);

//	NVIC_ClearPendingIRQ(RTC_IRQn);
	NVIC_ClearPendingIRQ(RTC_Seconds_IRQn);
//	NVIC_EnableIRQ(RTC_IRQn);
	NVIC_EnableIRQ(RTC_Seconds_IRQn);
	__enable_irq();

}

void RTC_IRQHandler()
{
	logger_log(INFO,"RTC_IRQHandler");
}

void RTC_Seconds_IRQHandler()
{
	__disable_irq();
	NVIC_ClearPendingIRQ(RTC_IRQn);
	logger_log(HEARTBEAT,"");
	GPIO_Red_Toggle();
	RTC_SR &= ~RTC_SR_TCE(1);
	RTC->TPR |= RTC_TPR_TPR(0x7BFF);
	RTC_SR |= RTC_SR_TCE(1);
	__enable_irq();
}

#else

#define G_Current_Time	time(NULL)

#endif

char* getcurrentTimeStampString()
{
	time_t t= G_Current_Time;
	if(t > 0)
	{
		char *timeStamp_string = ctime(&t);
		return timeStamp_string;
	}
	else
		return NULL;
}

char* getString_of_TimeStamp(time_t epochTime)
{
	if(epochTime > 0)
	{
		char *timeStamp_string = ctime(&epochTime);
		return timeStamp_string;
	}
	else
		return NULL;
}

uint32_t getTimeStamp()
{
	return G_Current_Time;

}
