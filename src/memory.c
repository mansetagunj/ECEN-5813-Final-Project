/**
* @file - memory.c
* @brief - Implementation file for the memory functions
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 02/10/2017
**/

#include "memory.h"
#include <malloc.h>
#include "debug.h"
#ifdef PLATFORM_KL25Z
#include "dma.h"

uint8_t dma_setValue = 0;
#endif

uint8_t* my_memmove(uint8_t * src, uint8_t * dst, size_t length)
{
	uint8_t *p_ret = NULL;
	if (NULL != dst && NULL != src)
	{
//		uint8_t *p_tempMem = (uint8_t*)malloc(sizeof(uint8_t)*length);
//		uint8_t *p_src = src;
		uint8_t *p_dst = dst;
		uint8_t *p_tempMem = src;

//		//Deep copy of the Src memory to a temp memory to handle any memory overlap issue
		size_t tempLength = length;
//		while (tempLength--)
//		{
//			*p_tempMem = *p_src++;
//			++p_tempMem;
//		}
//
//		p_tempMem -= length;	//bringing back the pointer to point to the start of the allocated mem
		tempLength = length;
		while (tempLength--)
		{
			*p_dst = *p_tempMem++;
			++p_dst;
		}

		//p_tempMem -= length;	//bringing back the pointer to point to the start of the allocated mem
		//free(p_tempMem);
		p_tempMem = NULL;

		p_ret = dst;
	}

	return p_ret;
}
//{
//    uint8_t *p_ret = NULL;
//    if (NULL != dst && NULL != src)
//    {
//        uint8_t *p_src = src;
//        uint8_t *p_dst = dst;
//        size_t length1=0;
//        while ((p_src!=p_dst) || (length!=length1)) {
//            ++p_src;
//            ++length1;
//        }
//        if (length1<length) {
//            p_dst+=length1;
//            while(length!=length1) {
//                *p_dst=*p_src;
//                length--;
//                p_dst++;
//                p_src++;
//            }
//        }
//        p_src=src;
//        p_dst=dst;
//        //p_tempMem -= length;  //bringing back the pointer to point to the start of the allocated mem
//        //tempLength = length;
//        while (length1--)
//        {
//            *p_dst = *p_src;
//            ++p_dst;
//            ++p_src;
//        }
//
//        //p_tempMem -= length;  //bringing back the pointer to point to the start of the allocated mem
//        //free(p_tempMem);
//        //p_tempMem = NULL;
//        print_memory(dst,length);
//        p_ret = dst;
//    }
//
//    return p_ret;
//}

uint8_t* my_memcpy(uint8_t * src, uint8_t * dst, size_t length)
{
	uint8_t *p_ret = NULL;
	if (NULL != dst && NULL != src)
	{
		uint8_t *p_src = src;
		uint8_t *p_dst = dst;

		//Deep copy of the Src memory to Dst memory
		size_t tempLength = length;
		while (tempLength--)
		{
			*p_dst = *p_src++;
			++p_dst;
		}

		p_ret = dst;
	}

	return p_ret;
}

int8_t* my_memset(uint8_t * src, size_t length, uint8_t value)
{
	if (NULL != src)
	{
		uint8_t *p_src = src;
		size_t tempLength = length;
		while (tempLength--)
		{
			*p_src = value;
			++p_src;
		}
		p_src = NULL;
	}	
	return (int8_t*)src;
}

uint8_t* my_memzero(uint8_t * src, size_t length)
{
	if (NULL != src)
	{
		uint8_t *p_src = src;
		size_t tempLength = length;
		while (tempLength--)
		{
			*p_src = 0;
			++p_src;
		}
		p_src = NULL;
	}
	return src;
}

uint8_t* my_reverse(uint8_t *src, size_t length)
{
	if (NULL != src && 0 < length)
	{
		uint8_t *forwardItr = src;
		uint8_t *backwardItr = src+length-1;

		//divide the length by two to get the midpoint value to use it in the loop
		int itr = length >> 1;
		while (itr && forwardItr && backwardItr)
		{
			//swapping routine
			uint8_t temp = *forwardItr;
			*forwardItr = *backwardItr;
			*backwardItr = temp;
			++forwardItr; //incrementing the forward pointer
			--backwardItr; //decrementing the reverse pointer
			--itr;
		}
		forwardItr = NULL;
		backwardItr = NULL;
	}
	return src;
}

int32_t* reserve_words(size_t length)
{
	int32_t *reservedMem = (int32_t*)malloc(sizeof(int32_t)*length);
	return reservedMem;
}

void free_words(int32_t *src)
{
	free(src);
}

#ifdef PLATFORM_KL25Z

int8_t memmove_dma(uint8_t *src, uint8_t *dst, size_t length)
{
	if(src && dst )
	{
//		uint8_t *p_tempMem= (uint8_t*)malloc(sizeof(uint8_t)*length);
//		if(p_tempMem == NULL)
//			return -1;
//		//Deep copy of the Src memory to a temp memory to handle any memory overlap issue
//		size_t itr = 0;
//		while (itr < length)
//		{
//			*(p_tempMem+itr) = *(src+itr);
//			++itr;
//		}

		DMA_Addresses_t addresses;
		addresses.Src_Add = (uint32_t)src;
		addresses.Dest_Add = (uint32_t)dst;
		addresses.NumberOfBytes = length;
//		if((length > 3) && ((length % 4) == 0))
		if(((length+4) % 4) == 0)
		{
			addresses.Dest_Size = DMA_32Bits;
			addresses.Src_Size = DMA_32Bits;
		}
//		else if((length > 1) && ((length %2) == 0))
		else if(((length+2) % 2) == 0)
		{
			addresses.Dest_Size = DMA_16Bits;
			addresses.Src_Size = DMA_16Bits;
		}
		else
		{
			addresses.Dest_Size = DMA_8Bits;
			addresses.Src_Size = DMA_8Bits;
		}
		addresses.SrcAddr_Inc = 1;
		addresses.DestAddr_Inc = 1;
		addresses.Start = 1;

		return dma_initiate(DMA_0, &addresses);
	}
	return -1;

}
int8_t memset_dma(uint8_t *src, size_t length, uint8_t value)
{
	if(src)
	{
		DMA_Addresses_t addresses;
		dma_setValue = value;
		addresses.Src_Add = (uint32_t)&dma_setValue;
		addresses.Dest_Add = (uint32_t)src;
		addresses.NumberOfBytes = length;
		addresses.Dest_Size = DMA_8Bits;
		addresses.Src_Size = DMA_8Bits;
		addresses.SrcAddr_Inc = 0;
		addresses.DestAddr_Inc = 1;
		addresses.Start = 1;
		return dma_initiate(DMA_0, &addresses);
	}
	return -1;
}
#else

int8_t memmove_dma(uint8_t *src, uint8_t *dst, size_t length)
{
	my_memmove(src, dst, length);
	return 0;
}
int8_t memset_dma(uint8_t *src, size_t length, uint8_t value)
{
	my_memset(src, length, value);
	return 0;
}

#endif
