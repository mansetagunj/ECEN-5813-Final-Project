/*
 * mcg.c
 *
 *  Created on: 27-Oct-2017
 *      Author: Gunj Manseta
 */

#include "MKL25Z4.h"
#include "mcg.h"
#include <time.h>

void mcg_Init()
{
	/*	Using the on reset default FEI mode having Internal clock reference on
	*	and sourcing the FLL. The MCGOUTCLK will be from FLL.
	*	Setting up clock to 48MHz using the MCG reg
	*	IRefClock - 32768Hz
	*	FLL Factor - 1463
	*	So, MCGFLLCLOCK - 32768*1463 = 47939584 ~48MhZ
	*/
	MCG->C4 |= MCG_C4_DRST_DRS(MCG_C4_DRST_DRS_48MHZ) | MCG_C4_DMX32(MCG_C4_DMX32_48MHZ);
}
