/*
 * dma.c
 *
 *  Created on: 30-Nov-2017
 *      Author: Gunj Manseta
 */


#include "dma.h"
#include "MKL25Z4.h"
#include "time_profiler.h"
#include "logger.h"

#define DMA_COUNT (4)

volatile DMA_state_t DMA_CurrentState[4] = {0};
volatile tickTime tickStart = 0;
volatile tickTime tickEnd = 0;

void dma_clockEnable()
{
	SIM->SCGC7 |= SIM_SCGC7_DMA(1);
}

void dma_clockDisable()
{
	SIM->SCGC7 &= ~(SIM_SCGC7_DMA_MASK);
	DMA_CurrentState[DMA_0] = DMA_Disabled;
	DMA_CurrentState[DMA_1] = DMA_Disabled;
	DMA_CurrentState[DMA_2] = DMA_Disabled;
	DMA_CurrentState[DMA_3] = DMA_Disabled;
}

int8_t dma_configure(DMA_t dma_n, DMA_Configure_t *DMA_config_data)
{
	if(dma_n < DMA_COUNT)
	{
		dma_clockEnable();
		DMA_DSR_BCR(dma_n) |= DMA_DSR_BCR_DONE(1);

		DMA_DCR(dma_n) = 0;
		DMA_DCR(dma_n) |= DMA_DCR_AA(DMA_config_data->AutoAlign);
		DMA_DCR(dma_n) |= DMA_DCR_EINT(DMA_config_data->EnableInterrupt);
		DMA_DCR(dma_n) |= DMA_DCR_CS(DMA_config_data->CycleSteal);
		DMA_DCR(dma_n) |= DMA_DCR_ERQ(DMA_config_data->EnablePeripheralReq);
		DMA_DCR(dma_n) |= DMA_DCR_D_REQ(DMA_config_data->D_REQ);

		NVIC_ClearPendingIRQ(dma_n);
		NVIC_EnableIRQ(dma_n);
		DMA_CurrentState[dma_n] = DMA_Ready;
		return 0;
	}
	else
	{
		return -1;
	}

}

int32_t dma_initiate(DMA_t dma_n, DMA_Addresses_t *DMA_addresses_data)
{
	if(dma_n < DMA_COUNT && ((DMA_CurrentState[dma_n] == DMA_Ready) || (DMA_CurrentState[dma_n] == DMA_Complete)) )
	{
		DMA_SAR(dma_n) 	=	DMA_addresses_data->Src_Add;
		DMA_DAR(dma_n)	=	DMA_addresses_data->Dest_Add;
		DMA_DSR_BCR(dma_n) |= DMA_DSR_BCR_BCR(DMA_addresses_data->NumberOfBytes);


		DMA_DCR(dma_n) &= ~DMA_DCR_SINC_MASK;
		DMA_DCR(dma_n) &= ~DMA_DCR_DINC_MASK;
		DMA_DCR(dma_n) &= ~DMA_DCR_SSIZE_MASK;
		DMA_DCR(dma_n) &= ~DMA_DCR_DSIZE_MASK;

		DMA_DCR(dma_n) |= (DMA_addresses_data->Src_Size < 3)? DMA_DCR_SSIZE(DMA_addresses_data->Src_Size) : DMA_DCR_SSIZE(DMA_8Bits);
		DMA_DCR(dma_n) |= (DMA_addresses_data->Dest_Size < 3)?DMA_DCR_DSIZE(DMA_addresses_data->Dest_Size): DMA_DCR_DSIZE(DMA_8Bits);

		DMA_DCR(dma_n) |= DMA_DCR_SINC(DMA_addresses_data->SrcAddr_Inc);
		DMA_DCR(dma_n) |= DMA_DCR_DINC(DMA_addresses_data->DestAddr_Inc);

		DMA_CurrentState[dma_n] = DMA_Busy;
		DMA_DCR_REG(DMA0,dma_n) |= DMA_DCR_START(DMA_addresses_data->Start);
		return 0;
	}
	else
	{
		return -1;
	}
}

int8_t dma_startTransfer(DMA_t dma_n)
{
	if(dma_n < DMA_COUNT && ((DMA_CurrentState[dma_n] == DMA_Ready) || (DMA_CurrentState[dma_n] == DMA_Complete)))
	{
		DMA_CurrentState[dma_n] = DMA_Busy;
		DMA_DCR_REG(DMA0,dma_n) |= DMA_DCR_START(1);
		return 0;
	}
	else
	{
		return -1;
	}
}

void DMA0_IRQHandler()
{
	tickEnd = profiler_getCurrentTick(tickEnd);
	__disable_irq();
	NVIC_ClearPendingIRQ(DMA0_IRQn);
	if( DMA_DSR_BCR(DMA_0) & ( DMA_DSR_BCR_CE_MASK | DMA_DSR_BCR_BES_MASK | DMA_DSR_BCR_BED_MASK ) )
	{
		DMA_CurrentState[DMA_0] = DMA_Error;
		//logger_log(ERROR,"ERROR DMA%d. DCR: 0x%x\r\n",DMA_0,DMA_DSR_BCR(DMA_0) & (DMA_DSR_BCR_CE_MASK | DMA_DSR_BCR_BES_MASK | DMA_DSR_BCR_BED_MASK));
	}
	else
		DMA_CurrentState[DMA_0] = DMA_Complete;

	DMA_DSR_BCR(DMA_0) |= DMA_DSR_BCR_DONE(1);
	__enable_irq();
}

void DMA1_IRQHandler()
{
	__disable_irq();
	NVIC_ClearPendingIRQ(DMA1_IRQn);
	if( DMA_DSR_BCR(DMA_1) & ( DMA_DSR_BCR_CE_MASK | DMA_DSR_BCR_BES_MASK | DMA_DSR_BCR_BED_MASK ) )
	{
		DMA_CurrentState[DMA_1] = DMA_Error;
		logger_log(ERROR,"ERROR DMA%d. DCR: 0x%x\r\n",DMA_1,DMA_DSR_BCR(DMA_1) & (DMA_DSR_BCR_CE_MASK | DMA_DSR_BCR_BES_MASK | DMA_DSR_BCR_BED_MASK));
	}
	else
		DMA_CurrentState[DMA_1] = DMA_Complete;

	DMA_DSR_BCR(DMA_1) |= DMA_DSR_BCR_DONE(1);
	__enable_irq();
}
void DMA2_IRQHandler()
{
	__disable_irq();
	NVIC_ClearPendingIRQ(DMA2_IRQn);
	DMA_DSR_BCR(DMA_2) |= DMA_DSR_BCR_DONE(1);
	DMA_CurrentState[DMA_2] = DMA_Complete;
	__enable_irq();
}
void DMA3_IRQHandler()
{
	__disable_irq();
	NVIC_ClearPendingIRQ(DMA3_IRQn);
	DMA_DSR_BCR(DMA_3) |= DMA_DSR_BCR_DONE(1);
	DMA_CurrentState[DMA_3] = DMA_Complete;
	__enable_irq();
}
