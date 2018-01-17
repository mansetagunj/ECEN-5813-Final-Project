#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef PLATFORM_KL25Z
	extern int printf(const char *fmt,...);
#else
#include <stdio.h>
#endif

#endif

