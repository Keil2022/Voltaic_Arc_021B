#ifndef 		__PWM_H
#define 	__PWM_H

#include	"SYSCFG.h"
#include "Hardward.c"

#define POUT	RC3
#define P1C		TRISC3
#define P1C_M	P1COE

#define PWM	1
#define IO		0

volatile u16 pwm_d=0;      //PWM脉冲宽度值

/*-------------------------------------------------
 *  函数名：pwm_duty_count
 *	功能：  将十位pwm_d的值赋值给CCPR1L:CCP1CON<5:4>
 *  输入：  无
 *  输出：  无
 --------------------------------------------------*/	
void pwm_duty_count (void)  
{    
	u16  lsb22;  
	u8 lsb23;            	     	   
	lsb22 = (u8) pwm_d & 0B00000011;
	CCP1CON =  CCP1CON & 0B11001111;
	lsb22 <<= 4;
	lsb23 = (u8)lsb22; 
	CCP1CON =  CCP1CON | lsb23; //赋值10Bit PWM脉冲宽度的低2位LSB
	lsb22 = pwm_d >> 2; 		//赋值10Bit PWM脉冲宽度的高8位MSB
	lsb23 = (u8)lsb22; 
	CCPR1L = lsb23;  
}
/*-------------------------------------------------
 *  函数名：PWM_INITIAL
 *	功能：  PWM初始化
 *  输入：  SET_PR2:周期
 *		   PWM周期 = (SET_PR2+1)*2*(1/Fosc)*T2分频比值
 *  输出：  无
 --------------------------------------------------*/
void PWM_INITIAL (u8 SET_PR2) 
{
	MSCKCON = 0B00000000; 		//Bit6->0,禁止PA4，PC5稳压输出
	                            //Bit5->0为TIMER2时钟为Fosc ; 1为TIMER2时钟为32MHZ
					   			//Bit4->0,禁止LVR  
	T2CON = 0B00000000;   		//Bit(1:0)=00;T2分频比为1:1
	/*
	//MSCKCON->5Bit=1时使用如下计算公式***************************
	PR2 = 15; 					//设置PWM的周期 = (PR2+1)*4*(1/Fosc*2)*T2分频比值
			                    //Tpwm = (15+1)*2*(1/16000000*2)*1 = 8us = 125KHZ
	*/
	//MSCKCON->5Bit=0时使用如下计算公式***************************
	PR2 = SET_PR2; //31			//设置PWM的周期 = (PR2+1)*2*(1/Fosc)*T2分频比值
			                    //Tpwm = (199+1)*2*(1/16000000)*1 = 50us = 20KHZ
	//CCP1CON = 0B10001100; 	//Bit(7:6)=10;P1A,P1B半桥输出带死区
					   			//Bit(5:4)=00;10Bit PWM占空比低2位
					   			//Bit(3:0)=1100;PWM模式，P1A和P1B为高电平有效
	CCP1CON = 0B10001101; 		//Bit(7:6)=00;P1A单输出,P1B,P1C,P1D位普通I/O口
					   			//Bit(5:4)=00;10Bit PWM占空比低2位
					   			//Bit(3:0)=1101;PWM模式，P1A高电平有效,P1B低电平有效

	pwm_duty_count();      		//PWM占空比计算 占空比pwm_d/((PR2+1)*4)

	PWM1CON = 0B10000001; 		//Bit7=1,自动关闭时PWM自动重启
					   			//死区时间=Bit<6:0>*(1/Fosc)*2
					   			//=1*(1/16000000)*2=0.125ns
	ECCPAS = 0B00001111;  		//Bit(6:4)=000,禁止自动关闭
					   			//Bit(3:2)=11,P1A,P1C关闭时为三态
					   			//Bit(1:0)=11,P1B,P1D关闭时为三态
	//PWM1AUX = 0B10000011; 	//Bit7=1,使能PWM辅助功能
					   			//Bit6=0,使能PWM连续输出
					   			//Bit5:2=0000,P1C,P1D,P1E,P1F引脚为IO
					   			//Bit1:0=11,P1A,P1B为半桥PWM输出
	PWM1AUX = 0B10000000; 		//Bit7=1,使能PWM辅助功能
					   			//Bit6=0,使能PWM连续输出
					   			//Bit5:2=0011,P1C,P1D为半桥PWM输出
					   			//Bit1:0=11,P1A,P1B,P1E,P1F引脚为IO
	TMR2IF = 0;           		//清T2中断标志位  
	//TMR2ON = 1;           	//使能开启T2 
}

void PWM_ON(void)
{
	P1C = 0;				//输出
    P1C_M = PWM;	//PWM输出模式
    TMR2ON = 1;		//TIM2 ON
}

void PWM_OFF(void)
{
	P1C = 0;				//输出
    P1C_M = IO;		//PWM输出模式
    POUT = 0;
    TMR2ON = 0;		//TIM2 OFF
}

void PWM_20kHz(void)
{
	pwm_d = 121;
	PR2 = 49;
    pwm_duty_count();
}

void PWM_12kHz(void)
{
	pwm_d = 200;
	PR2 = 82;
    pwm_duty_count();
}

void PWM_Init(void)
{
	TRISC3 = 0;	//输出模式	//0-输出；1-输入；
}

#endif
