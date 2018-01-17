/*
 * platform.c
 *
 *  Created on: 30-Oct-2017
 *      Author: Gunj Manseta
 */

#include "platform.h"

#ifdef PLATFORM_KL25Z

#include "uart0.h"
#include "stdarg.h"

int printf(const char *fmt,...)
{
	va_list args;
	va_start(args, fmt);
	UART0_printf((char*)fmt, args);
	va_end(args);
	return 0;
}
#endif
