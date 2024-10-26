#include "stm32f4xx.h"                  // Device header
#include "EXTI_Control.h"
void EXTI_Control_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_Initstructure;
	NVIC_InitTypeDef NVIC_Initstructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);
	
	EXTI_Initstructure.EXTI_Line = EXTI_Line2;
	EXTI_Initstructure.EXTI_LineCmd = ENABLE;
	EXTI_Initstructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_Initstructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_Initstructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_Initstructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Initstructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Initstructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Initstructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_Initstructure);
}
//void EXTI2_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line2) == SET)
//	{
//		GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
//		GPIO_ToggleBits(GPIOF,GPIO_Pin_10);
//		GPIO_ToggleBits(GPIOE,GPIO_Pin_13);
//		GPIO_ToggleBits(GPIOE,GPIO_Pin_14);

//		EXTI_ClearITPendingBit(EXTI_Line2);
//	}
//}
