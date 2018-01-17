/*
 * time_profiler.c
 *
 *  Created on: 01-Dec-2017
 *      Author: Gunj Manseta
 */

#include "time_profiler.h"

volatile uint8_t tick_overflow = 1;

#ifdef PLATFORM_KL25Z

#include "MKL25Z4.h"

#define G_SYSTICKS (SysTick->VAL)

void profiler_setup()
{
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_IRQHandler()
{
	__disable_irq();
	tick_overflow++;
	__enable_irq();
}

tickTime profiler_getTickDiff(tickTime since)
{
	tickTime now = G_SYSTICKS;
	return (((since - now) >= 0) ? (since - now) : (since + ((1+ TICK_MAX)*tick_overflow) - now));
}

tickTime profiler_getCurrentTick()
{
	//tick_overflow = 1;
	return G_SYSTICKS;
}

#else

//#define G_SYSTICKS clock()
#define G_SYSTICKS timevalue.tv_usec

tickTime profiler_getTickDiff(tickTime since)
{
	gettimeofday(&timevalue,NULL);
	tickTime now = G_SYSTICKS;
	return (((now - since) >= 0) ? (now - since) : (now + ((1+ TICK_MAX)*tick_overflow) - since));
}

tickTime profiler_getCurrentTick()
{
	//tick_overflow = 1;
	gettimeofday(&timevalue,NULL);
	return G_SYSTICKS;
}

#endif



float profiler_getTime_us(tickTime ticks)
{
	return ((((float)ticks)*(1000000.0))/CLK_PER_SEC);
}


