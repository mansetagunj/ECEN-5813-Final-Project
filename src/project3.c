/**
* @file - project3.c
* @brief - File calling all the functionalities for project3
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 8 Dec 2017
**/

#include "project3.h"
#include "memory.h"
#include "time_profiler.h"
#include "conversion.h"
#include <string.h>
#include "platform.h"
#include "logger.h"
#include "data_processing.h"
#include "timestamp.h"

#ifdef PLATFORM_KL25Z
#include "MKL25Z4.h"
#include "uart0.h"
#include "timer0.h"
#include "dma.h"
#include "gpio.h"
#include "mcg.h"
#include "spi.h"
#include "nordic_driver.h"

extern uint32_t __HeapLimit;
extern uint32_t STACK_SIZE;
extern uint32_t HEAP_SIZE;

void Nordic_Test();

#else
CB_t processingBuffer;
tickTime tickStart = 0;
tickTime tickEnd = 0;
#endif


void profiler_Test();
void profile_memoryFunctions(uint32_t Data_Size);

void project3()
{

#ifdef PLATFORM_KL25Z
	__disable_irq();
	mcg_Init();
	UART0_configure(BAUD_115200);
	rtc_init();
	logger_log(INFO,"CLOCK INIT");
	logger_log(INFO,"UART0 INIT");
	logger_log(INFO,"BUILD EPOCH TIME: %u",BUILD_EPOCH_TIME);
	logger_log(LOGGER_INITIALZED,"");

	GPIO_Red_Led_En();
	GPIO_Red_Off();

	logger_log(GPIO_INITIALZED,"");

	//timer0_configure();
	logger_log(INFO,"MEMORY - HEAP SIZE: 0x%x",&HEAP_SIZE);
	logger_log(INFO,"MEMORY - STACK SIZE: 0x%x",&STACK_SIZE);
	logger_log(INFO,"MEMORY - HEAP END: 0x%x",&__HeapLimit);

	__enable_irq();

#else
	CB_init(&processingBuffer,64);
	uint8_t data;
	uint8_t processDataNow = 0;
#endif

	logger_log(SYSTEM_INITIALIZED,"");
#ifdef PLATFORM_KL25Z
	Nordic_Test();
#endif


	profiler_Test();

	while(1)
	{
		if(processDataNow)
		{
			processDataNow = 0;
#ifdef PLATFORM_KL25Z
			processData(UART0_RX_buffer);
		}
#else
			processData(&processingBuffer);
		}
		scanf("%c",&data);
		CB_buffer_add_item(&processingBuffer,data);
		logger_log(DATA_RECEIVED,"%c",data);
		(data == '~') ? logging = 0 : 0;
		(data == '!') ? logging = 1 : 0;
		(data == '|') ? log_format = BINARY_LOGGER : 0;
		(data == '/') ? log_format = ASCII_LOGGER : 0;
		(data == '\n') ? processDataNow = 1: 0;

#endif
	}

}


void profile_memoryFunctions(uint32_t Data_Size)
{
	logger_log(PROFILING_STARTED,"To test %d of transfer",Data_Size);
	uint8_t samples_i = 0;

	uint8_t *src = (uint8_t*)malloc(Data_Size);
	uint8_t *dst = (uint8_t*)malloc(Data_Size);
//	uint8_t src[Data_Size];
//	uint8_t dst[Data_Size];
#ifdef PLATFORM_KL25Z
	if(((uint32_t)&__HeapLimit < (uint32_t)(src+Data_Size)) || ((uint32_t)&__HeapLimit < (uint32_t)(dst+Data_Size)))
		logger_log(WARNING,"HEAP AND STACK MIGHT INTERSECT");
#endif
	if(src == NULL)
	{
		logger_log(ERROR,"MALLOC FAILED: SRC");
		return;
	}
	if(dst == NULL)
	{
		free(src);
		logger_log(ERROR,"MALLOC FAILED: DEST");
		return;
	}

	while( samples_i < 1)
	{
		memset(src,0,Data_Size);
		memset(dst,0,Data_Size);
		tickStart = 0;
		tickEnd = 0;
		profiler_setup();
		tickStart = profiler_getCurrentTick();
		my_memset(src,Data_Size,'S');
		tickTime diff = profiler_getTickDiff(tickStart);
		//UART0_putstr("my_memset Ticks: ");
		//uint8_t len  = sprintf(str,"%u\tTime: %f us",diff,profiler_getTime_us(diff));
		//UART0_printf("%u\tTime: %f us",diff,profiler_getTime_us(diff));
		//UART0_putstr(str);
		//UART0_NEWLINE;
		logger_log(PROFILING_RESULT,"my_memset Time: %f us",profiler_getTime_us(diff));

		tickStart = 0;
		tickEnd = 0;
		//profiler_setup();
		tickStart = profiler_getCurrentTick();
		my_memmove(src,dst,Data_Size);
		diff = profiler_getTickDiff(tickStart);
		logger_log(PROFILING_RESULT,"my_memmove Time: %f us",profiler_getTime_us(diff));

		tickStart = 0;
		tickEnd = 0;
		//profiler_setup();
		tickStart = profiler_getCurrentTick();
		if(memset_dma(src,Data_Size,'X') == -1)
			logger_log(ERROR,"memset_dma failed");

#ifdef PLATFORM_KL25Z
		//while(DMA_CurrentState[DMA_0] != DMA_Complete && DMA_CurrentState[DMA_0] != DMA_Error);
		while(DMA_CurrentState[DMA_0] == DMA_Busy);
		diff = tickStart - tickEnd;
		if(DMA_CurrentState[DMA_0] == DMA_Error)
		{
			logger_log(ERROR,"memset_dma : Error in transfer on DMA0");
			DMA_CurrentState[DMA_0] = DMA_Ready;
		}
		else
		{
			logger_log(PROFILING_RESULT,"memset_dma Time: %f us",profiler_getTime_us(diff));
		}
#else
		diff = profiler_getTickDiff(tickStart);
		logger_log(PROFILING_RESULT,"my_memset Time: %f us",profiler_getTime_us(diff));
#endif

		tickStart = 0;
		tickEnd = 0;
		//profiler_setup();
		tickStart = profiler_getCurrentTick();
		if(memmove_dma(dst,src,Data_Size) == -1)
			logger_log(ERROR,"memmovet_dma failed");
#ifdef PLATFORM_KL25Z
		//while(DMA_CurrentState[DMA_0] != DMA_Complete && DMA_CurrentState[DMA_0] != DMA_Error);
		while(DMA_CurrentState[DMA_0] == DMA_Busy);
		diff = tickStart - tickEnd;
		if(DMA_CurrentState[DMA_0] == DMA_Error)
		{
			logger_log(ERROR,"memmove_dma : Error in transfer on DMA0");
			DMA_CurrentState[DMA_0] = DMA_Ready;
		}
		else
		{
			logger_log(PROFILING_RESULT,"memmove_dma Time: %f us",profiler_getTime_us(diff));
		}
#else
		diff = profiler_getTickDiff(tickStart);
		logger_log(PROFILING_RESULT,"my_memmove Time: %f us",profiler_getTime_us(diff));
#endif

		tickStart = 0;
		tickEnd = 0;
		//profiler_setup();
		tickStart = profiler_getCurrentTick();
		memset(src,'Z',Data_Size);
		diff = profiler_getTickDiff(tickStart);
		logger_log(PROFILING_RESULT,"memset Time: %f us",profiler_getTime_us(diff));

		tickStart = 0;
		tickEnd = 0;
		//profiler_setup();
		tickStart = profiler_getCurrentTick();
		memmove(src,dst,Data_Size);
		diff = profiler_getTickDiff(tickStart);
		logger_log(PROFILING_RESULT,"memmove Time: %f us",profiler_getTime_us(diff));

		tickStart = 0;
		tickEnd = 0;
		samples_i++;

	}

	free(src);
	free(dst);

	logger_log(PROFILING_COMPLETED,"");
}


void profiler_Test()
{
#ifdef PLATFORM_KL25Z
	DMA_Configure_t config;
	config.AutoAlign=1;
	config.CycleSteal=0;
	config.D_REQ=0;
	config.EnableInterrupt=1;
	config.EnablePeripheralReq = 0;
	if(dma_configure(DMA_0,&config) == -1)
		logger_log(ERROR,"DMA_0 Configuration.");
#endif

//	profile_memoryFunctions(1);
//	profile_memoryFunctions(2);
//	profile_memoryFunctions(3);
//	profile_memoryFunctions(4);
	profile_memoryFunctions(10);
	profile_memoryFunctions(100);
	profile_memoryFunctions(500);
	profile_memoryFunctions(5000);

}

#ifdef PLATFORM_KL25Z
void Nordic_Test()
{
	NRF_moduleInit();

	logger_log(INFO, "SPI Initialized");
	logger_log(INFO,"Nordic Initialized");
	logger_log(INFO,"Nordic Test");
	uint8_t sendValue = 0x0F;
	uint8_t readValue = 0;
//	NRF_write_config(sendValue);
	readValue = NRF_read_config();
	if(readValue == sendValue)
	{
		logger_log(INFO,"Write/Read Config Value Matched");
		logger_log(INFO,"Sent: 0x%x",sendValue);
		logger_log(INFO,"Recv: 0x%x",readValue);
	}

//	uint8_t sendAddr[5] = {0xBA,0x56,0xBA,0x56,0xBA};
	uint8_t sendAddr[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
	logger_log(INFO,"TX ADDRESSES SET: 0x%x%x%x%x%x",sendAddr[0],sendAddr[1],sendAddr[2],sendAddr[3],sendAddr[4]);
	NRF_write_TX_ADDR(sendAddr);
	uint8_t *readAddr = (uint8_t*)malloc(5);
	NRF_read_TX_ADDR(readAddr);
	logger_log(INFO,"TX ADDRESSES GET: 0x%x%x%x%x%x",readAddr[0],readAddr[1],readAddr[2],readAddr[3],readAddr[4]);
	free(readAddr);

	sendValue = 0x02;
	NRF_write_rf_ch(sendValue);
	readValue = NRF_read_rf_ch();
	if(readValue == sendValue)
	{
		logger_log(INFO,"Write/Read RF CH Value Matched");
		logger_log(INFO,"Sent: 0x%x",sendValue);
		logger_log(INFO,"Recv: 0x%x",readValue);
	}

	sendValue = 0x0F;
	NRF_write_rf_setup(sendValue);
	readValue = NRF_read_rf_setup();
	if(readValue == sendValue)
	{
		logger_log(INFO,"Write/Read RF Setup Value Matched");
		logger_log(INFO,"Sent: 0x%x",sendValue);
		logger_log(INFO,"Recv: 0x%x",readValue);
	}

	NRF_Mode_t mode = NRF_Mode_RX;
	logger_log(INFO,"Configuring NRF in %d mode",mode);
	NRF_mode_configure(mode);
	uint8_t Data[2] = {0};
	NRF_read_data(Data,2);
	logger_log(INFO,"Nordic Data Recvd: 0x%x, 0x%x", Data[0],Data[1]);


//	NRF_Mode_t mode = NRF_Mode_TX;
//	logger_log(INFO,"Configuring NRF in %d mode",mode);
//	NRF_mode_configure(mode);
//	uint8_t Data[2] = {0xAA,0x55};
//	NRF_transmit_data(Data,2);
//	logger_log(INFO,"Nordic Data Sent: 0x%x, 0x%x", Data[0],Data[1]);

	logger_log(INFO,"Nordic Test End");

	NRF_moduleDisable();
}
#endif
