/**
* @file - dma.h
* @brief - Header file for the HAL for DMA
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 8 Dec 2017
**/

#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>
#include "time_profiler.h"

/**
* @brief - Enum for DMA selection
*/
typedef enum{

	DMA_0 = 0,
	DMA_1,
	DMA_2,
	DMA_3
}DMA_t;

/**
* @brief - Enum for DMA data transfer width
*/
typedef enum{
	DMA_32Bits = 0,
	DMA_8Bits,
	DMA_16Bits
}DMA_transfer_DataSize;

/**
* @brief - Structure  for configuration of the DMA
*/
typedef struct{

	uint8_t EnableInterrupt:1;
	uint8_t EnablePeripheralReq:1;
	uint8_t CycleSteal:1;
	uint8_t AutoAlign:1;
	uint8_t D_REQ:1;				//If 1 - DMA cleares the ERQ bit after transfer completes
}DMA_Configure_t;

/**
* @brief - Structure  for setting up the addresses for the DMA transfer
*/
typedef struct{

	uint32_t Src_Add;
	uint32_t Dest_Add;
	uint32_t NumberOfBytes:20;
	uint32_t Src_Size:2;
	uint32_t Dest_Size:2;
	uint32_t SrcAddr_Inc:1;
	uint32_t DestAddr_Inc:1;
	uint32_t Start:1;				//Transfer Starts immediately after configuration completes
}DMA_Addresses_t;

typedef enum{

	DMA_Initial = 0,
	DMA_Disabled,
	DMA_Ready,
	DMA_Busy,
	DMA_Complete,
	DMA_Error
}DMA_state_t;

/**
* @brief - Stores the DMA state for 4 different DMA which can be accessed using the DMA_t
*/
volatile DMA_state_t DMA_CurrentState[4];

volatile tickTime tickStart;
volatile tickTime tickEnd;

/**
* @brief - Enables the clock for DMA
* @return void
**/
void dma_clockEnable();

/**
* @brief - Disables the clock for DMA
* @return void
**/
void dma_clockDisable();

/**
* @brief - Configures the specified DMA depending on the configuration structure
* @param - DMA_t
* @param - DMA_Configure_t*
* @return int8_t return -1 for error
**/
int8_t dma_configure(DMA_t dma_n, DMA_Configure_t *DMA_config_data);


/**
* @brief - Initiates the specified DMA transfer.
* 			Depending on the start flag, the transfer starts or waits for the dma_start to be called
* @param - DMA_t RXBuffer
* @param - DMA_Addresses_t*
* @return int32_t returns -1 for error
**/
int32_t dma_initiate(DMA_t dma_n, DMA_Addresses_t *DMA_addresses_data);

/**
* @brief - Starts the specified DMA transfer.
* 			Should be called after configuration and initiation is done and the start flag in the
* 			DMA_Addresses_t structure is false
* @param - DMA_t
* @return int8_t return -1 for error
**/
int8_t dma_startTransfer(DMA_t dma_n);

#endif /* __DMA_H__ */
