/**
* @file - timestamp.h
* @brief - Header file for the functions returning timestamps in a particular format
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 8 Dec 2017
**/

#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__

#include <stdint.h>
#include <time.h>

#ifdef PLATFORM_KL25Z

/**
* @brief - Initialiaze the RTC module
* @return void
**/
void rtc_init();

#else
static inline void rtc_init(){}
#endif

/**
* @brief - Return the value in the RTC Seconds register
* @return uint32_t
**/
uint32_t getTimeStamp();

/**
* @brief - Get the current Timestamp in string format
* @return char *
**/
char* getcurrentTimeStampString();

/**
* @brief - Get the current Timestamp in string format by providing an epoch value
* @param - epochTime time_t
* @return char *
**/
char* getString_of_TimeStamp(time_t epochTime);


#endif /* __TIMESTAMP_H__ */
