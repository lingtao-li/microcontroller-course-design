#include "stm32f4xx.h"                  // Device header
#include "Serial.h"
#include "LED.h"
#include "string.h"
#include "gec_delay.h"
int i = 0;
char ch = '\0';
uint16_t Serial_RxData;
char Serial_RxDataArray[5];
void Serial_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);  
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
}

void Serial_1_SetBaud(int num)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = num;
	USART_Init(USART1, &USART_InitStructure);
}

uint16_t Serial_GetRxData(void)
{
	return Serial_RxData;
}
void Serial_SendByte(uint16_t Byte)
{
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1 ,USART_FLAG_TC) == RESET);
}

void Serial_SendString(char *Str)
{
	uint8_t i;
	for (i = 0; Str[i] != '\0'; i ++)
	{
		Serial_SendByte(Str[i]);
	}
}

int String_Compare(char *Str)
{
//	uint8_t t;
	int flag1 = 0;
	int flag2 = 0;
	char *a = "ON";
	char *b = "OFF";
//	for (t = 0; Str[t] != '\0'; t++)
//	{
		if(strncmp(a,Str,2) == 0)
		{
			flag1 = 1;
		}
		if(strncmp(b,Str,3) == 0)
		{
			flag2 = 1;
		}
//		Str[t] = 0;
//	}
	i = 0;
	ch = '\0';
	if(0 == flag1 && 0 == flag2)
	{
		return 0;
	}
	else
	{
		if(1 == flag1)
		{
			return 1;
		}
		else if(1 == flag2)
		{
			return 2;
		}
	}
	return 3;
}
//void USART1_IRQHandler(void)
//{
//	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//	{
//		if((ch = USART_ReceiveData(USART1)) != '\n')
//		{
//			Serial_RxDataArray[i] = ch;
//			i++;
//		}
//		else
//		{
//			Serial_RxDataArray[i] = '\0';
//			Serial_SendString(Serial_RxDataArray);
//					switch(String_Compare(Serial_RxDataArray))
//			{
//				case 0 :
//					   GPIO_SetBits(GPIOF,GPIO_Pin_8);
//				     gec_delay_ms(1000);
//						 GPIO_ResetBits(GPIOF,GPIO_Pin_8);
//				    break;
//				case 1 : 
//						 Lit_led(1,1);
//				     Lit_led(2,1);
//						 Lit_led(3,1);
//						 Lit_led(4,1);
//						break;
//				case 2 : 
//						 Lit_led(1,0);
//				     Lit_led(2,0);
//						 Lit_led(3,0);
//						 Lit_led(4,0);
//						break;
//				case 3 : 
//						break;
//				default : break;
//			}
//		}
//		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//	}
//}
