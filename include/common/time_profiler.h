/**
* @file - time_profiler.h
* @brief - Header file for the time profiler
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 8 Dec 2017
**/

#ifndef __TIME_PROFILER_H__
#define __TIME_PROFILER_H__

#include <stdint.h>

#ifdef PLATFORM_KL25Z

#include "mcg.h"

typedef uint32_t tickTime ;

#define CLK_PER_SEC	FLL_CLK
#define TICK_MAX (16777215)

/**
* @brief - Initialization routine for Systick
* @return void
**/
void profiler_setup();

#else

#include "time.h"
#include "sys/time.h"
typedef clock_t tickTime ;
#define TICK_MAX (4294967295)
#define CLK_PER_SEC	(CLOCKS_PER_SEC)
struct timeval timevalue;
static inline void profiler_setup(){}
#endif

volatile uint8_t tick_overflow;

/**
* @brief - Get the current Systick value
* @return tickTime
**/
tickTime profiler_getCurrentTick();

/**
* @brief - Get the number of ticks since a particular instance
* @param - since tickTime
* @return tickTime
**/
tickTime profiler_getTickDiff(tickTime since);

/**
* @brief - Get the time in microseconds by providing the number of ticks
* @param - ticks tickTime
* @return float
**/
float profiler_getTime_us(tickTime ticks);

#endif /* __TIME_PROFILER_H__ */
