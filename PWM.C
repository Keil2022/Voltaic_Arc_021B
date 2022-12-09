#ifndef 		__PWM_H
#define 	__PWM_H

#include	"SYSCFG.h"
#include "Hardward.c"

#define POUT	RC3

void PWM_Init(void)
{
	TRISC3 = 0;	//输出模式	//0-输出；1-输入；
    
	
}

#endif
