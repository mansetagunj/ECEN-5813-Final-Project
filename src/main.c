/**
* @file - main.c
* @brief - Contains the entry point of the program which tests various functionalities
* 
* @author Gunj/Ashish University of Colorado Boulder
* @date	02/10/2017
**/

#ifdef PROJECT1
#include "project1.h"
#endif // PROJECT1

#ifdef PROJECT2
#include "project2.h"
#endif // PROJECT2

#ifdef HW5
#include "hw5.h"
#endif // HW5

#ifdef PROJECT3
#include "project3.h"
#endif // PROJECT3

int main()
{
#ifdef PROJECT1
	project1();
#endif // PROJECT1

#ifdef PROJECT2
	project2();
#endif // PROJECT2

#ifdef HW5
	hw5();
#endif // PROJECT2

#ifdef PROJECT3
	project3();
#endif

	return 0;
}
