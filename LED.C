#ifndef __LED_H
#define __LED_H

#include	"SYSCFG.h"
#include "Hardward.c"

#define LED		RC2

void LED_Init(void)
{
	TRISC2 = 0;	//0:Êä³öÄ£Ê½
	
    LED = OFF;
}

#endif

