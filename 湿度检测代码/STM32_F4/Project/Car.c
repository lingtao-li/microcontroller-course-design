#include "Car.h"
#include "stm32f4xx.h"                  // Device header
#include "gec_delay.h"
void CarInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = D0 | D1 | D2 | D3;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
void Car_test(int num,int flag)
{

	switch(num)
	{
		case 0 : 
					if(1 == flag)
				{
					GPIO_SetBits(WHL,D0);
				}
				else
				{
					GPIO_ResetBits(WHL,D0);
				}
				break;
		case 1 : 
					if(1 == flag)
				{
					GPIO_SetBits(WHL,D1);
				}
				else
				{
					GPIO_ResetBits(WHL,D1);
				}
		case 2 : 
					if(1 == flag)
				{
					GPIO_SetBits(WHL,D2);
				}
				else
				{
					GPIO_ResetBits(WHL,D2);
				}
				break;
		case 3 : 
					if(1 == flag)
				{
					GPIO_SetBits(WHL,D3);
				}
				else
				{
					GPIO_ResetBits(WHL,D3);
				}
				break;
		default : break;
	}
}
void Car_PWM(int n)
{
		int i=100;
		GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9);
	while(i!=0)
	{
		gec_delay_us(n);
		GPIO_ToggleBits(GPIOC,GPIO_Pin_7);
		gec_delay_us(10-n);
		GPIO_ToggleBits(GPIOC,GPIO_Pin_7);
		i--;
	}
		GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9);
}
