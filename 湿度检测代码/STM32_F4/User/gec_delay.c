/*****************************************************************************************
* 本程序只供学习使用,未经作者许可,不得用于其它任何用途
* STM32F407开发板(GEC_M4 8M晶振版)
* delay延时实现代码
* 广州粤嵌通信科技股份有限公司湖南分公司
* 创建日期:2020-04-15
* 版本: V1.0
* 
*****************************************************************************************/
#include "stm32f4xx.h"

unsigned int delay_time = 0;
#define rSysTickCTL	*((volatile unsigned long *)0xE000E010)

void SysTick_Handler(void)
{
	if(delay_time > 0)
	{
		delay_time--;
	}
}

/*
	任何一个中断处理函数中,都不能用定时器实现的延时
*/
void gec_delay_ms(unsigned int n)
{
	delay_time = n*1000;
	SysTick_Config(SystemCoreClock/1000000);	//设置为ms级Systick
	NVIC_SetPriority(SysTick_IRQn,1);			//将SysTick的中断优先级设置为最高
	while(delay_time != 0);	
	rSysTickCTL &= ~1;
}

void gec_delay_us(unsigned int n)
{
	delay_time = n;
	SysTick_Config(SystemCoreClock/1000000);	//设置为ms级Systick
		
	while(delay_time != 0);	
	rSysTickCTL &= ~1;
}



