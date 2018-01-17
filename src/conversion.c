/**
* @file - conversion.c
* @brief - Implementation file for the data conversion functions
*
* @author Gunj/Ashish University of Colorado Boulder
* @date 02/10/2017
**/

#include "conversion.h"
#include "memory.h"
#include <malloc.h>

#define SIGN_MASK (0x80000000)
#define ASCII_0 (0x30)
#define ASCII_7 (0x37)
#define ASCII_9 (0x39)

/**
* @brief - (Internal function) Raises 'base' to power 'pow'  
* Takes unsigned base and power to return an unsigned integer
* @param - base uint32_t
* @param - pow	uint8_t
* @return uint32_t
**/
uint32_t power(uint32_t base, uint8_t pow) {
	uint32_t op=1;
	while (pow>0) {
		op*=base;
		--pow;
	}
	return op;
}

uint8_t my_itoa(int32_t data, uint8_t * ptr, uint32_t base)
{
	uint8_t length=0;
        uint8_t * temp= (uint8_t *) malloc (sizeof(uint8_t)*33);
	uint8_t * temp_copy= temp;
        //Start with a '-', if the sign bit is set
	if (data & SIGN_MASK) {
                *ptr='-';
                ++ptr;
		//Perform 2's complement for the negative integer
		data=(~data)+1;
		++length;
        }
        do {    //Repeatedly divide by base and collect the remainders in temp
                *temp= data%base;
                data= data/base;
                ++length;
                ++temp;
        } while(data);
        --temp;
	//Reverse the order of characters in temp to obtain the actual ASCII string
        for (uint8_t i=0;i<length;i++) {
		if (*temp>9) {
			//ASCII conversion for letters A through F
			*ptr=(*temp)+ASCII_7;
			++ptr;
		}
		else if (*temp>=0 && *temp<9) {
			//ASCII conversion for digits 0 through 9
			*ptr=(*temp)+ASCII_0;
			++ptr;
		}
                --temp;
        }
	free(temp_copy);
	//Terminate with a null character
        *ptr='\0';
        return length;
}

int32_t my_atoi(uint8_t * ptr, uint8_t digits, uint32_t base)
{
	int32_t result=0;
	uint8_t neg_flag=0;
	//Set the negative flag if the ASCII string starts with '-'
	if (*ptr=='-') {
		neg_flag=1;
		--digits;
		++ptr;
	}
	while (digits>0) {
		//Reverse conversion from ASCII to Hex for A through F
		if (*ptr>ASCII_9)
			(*ptr)-=ASCII_7;
		//Reverse conversion from ASCII to Hex for 0 through 9
		else
			(*ptr)-=ASCII_0;
		result+= (int32_t)(*ptr) * (power(base,digits-1));
		--digits;
		++ptr;
	}
	//Perform 2's complement on the result if the negative flag is set
	if (neg_flag) {
		result=(~result)+1;
	}
	return result;
}

int8_t big_to_little32(uint32_t * data, uint32_t length)
{
	int8_t *p_reverse= NULL;
	int8_t p_ret = 1;	
	for (uint8_t i=0;i<length;i++) {
		p_reverse = (int8_t*)my_reverse((uint8_t*)data, 4);
	}
	if(p_reverse)
		p_ret = 0;
	return p_ret;
}

int8_t little_to_big32(uint32_t * data, uint32_t length)
{
	int8_t *p_reverse= NULL;
	int8_t p_ret = 1;	
	for (uint8_t i=0;i<length;i++) {
		p_reverse = (int8_t*)my_reverse((uint8_t*)data, 4);
	}
	if(p_reverse)
		p_ret = 0;
	return p_ret;
}
