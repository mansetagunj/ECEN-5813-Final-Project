/**
* @file - mcg.h
* @brief - Header file for the HAL for Multipurpose clock generator
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 27/10/2017
**/

#ifndef __MCG_H__
#define __MCG_H__

//Values for FLL Factor -1464 and DCO Range - 48MHz
#define MCG_C4_DRST_DRS_48MHZ	(1)
#define MCG_C4_DMX32_48MHZ		(1)
#define INTERNAL_CLOCK			(32768)
#define FLL_FACTOR				(1463)
#define FLL_CLK					(47939584)

/**
* @brief	Initializes the MCG.
* 			Using the on reset default FEI mode having Internal clock reference on
*			and which also sources the FLL. The MCGOUTCLK will be from FLL.
*			Setting up clock to 48MHz.
*			IRefClock - 32768Hz
*			FLL Factor - 1463
*			So, MCGFLLCLOCK - 32768*1463 = 47939584 ~48MhZ
*
*/
void mcg_Init();

#endif /* __MCG_H__ */
