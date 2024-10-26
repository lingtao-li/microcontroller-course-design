
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "gec_delay.h"
#include "GY39.h"
int zmq;
int lux;
int temp,pre,hum,height;


void debug_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin			=	GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode			=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd			=	GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	

	NVIC_InitStruct.NVIC_IRQChannel			=	USART1_IRQn;	
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART1,ENABLE);

}


void debug_send_byte(char ch)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) != SET);	//???????????
	USART_SendData(USART1,ch);									//?????1???
}


void debug_send_str(char* s,int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		debug_send_byte(s[i]);
	}
}


char recv_buf[512] = {0}; 	//?????????
int  recv_len   = 0;	  	//????????; 
int  recv_cnt	= 0;      	//????
int  recv_flag = 0;		  	// ??????   		1????    	0???
void USART1_IRQHandler(void)
{
	unsigned char ch;

	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		ch = USART_ReceiveData(USART1);	
		//debug_send_byte(ch);
		recv_buf[recv_cnt]	=	ch;		
		if(recv_buf[recv_cnt-1]=='\r'&& ch == '\n' || recv_cnt==510) 
		{//????
			recv_flag = 1;
			recv_len  = recv_cnt;
			recv_cnt  = 0;
		}
		else
		{
			recv_cnt++;
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

void usart2_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin			=	GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode			=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd			=	GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						//???
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			//???8
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				//????
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				//???1
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	//???????
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	//????
	USART_Init(USART2, &USART_InitStruct);

	/*
		3.????(??????)
	*/
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//???????????? ==>????????????

	/*
		4.??NVIC(????????NVIC)
	*/
	NVIC_InitStruct.NVIC_IRQChannel			=	USART2_IRQn;	//usart1?????
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/*
		5.????
	*/
	USART_Cmd(USART2,ENABLE);
}


/*************************GY39????****************************/
/*************************??2??*************************************/

unsigned char u2_recv[256] = {0};	/*????2??????*/
int u2_recv_count = 0;				/*????2????????*/
int u2_recv_flag = 0;				/*??????2???????? 1???? 0?????*/
//void USART2_IRQHandler(void)
//{
//	unsigned char ch;
//	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)//????????
//	{
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		ch = USART_ReceiveData(USART2);		//???2 ????
//		u2_recv[u2_recv_count] = ch;
//		u2_recv_count++;
//        //????????
//		if(u2_recv[2]==0x15 && u2_recv_count==8)
//		{
//			lux = u2_recv[4]<<24 | u2_recv[5]<<16 | u2_recv[6]<<8 | u2_recv[7];
//			u2_recv_flag = 1;
//			
//		}
//        //????????????????
//		else if(u2_recv[2]==0x45 &&  u2_recv_count==14)
//		{
//			temp = u2_recv[4]<<8 | u2_recv[5];
//			pre = u2_recv[6]<<24 | u2_recv[7]<<16 | u2_recv[8]<<8 | u2_recv[9];
//			hum = u2_recv[10]<<8 | u2_recv[11];
//			height = u2_recv[12]<<8 | u2_recv[13];
//			u2_recv_flag = 1;
//		}		
//		
//	}
//}
/*
	u2_send_byte ??2??1???
*/
void u2_send_byte(unsigned char ch)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);	//???????????
	USART_SendData(USART2,ch);
}

/*
	u2_send_str ??2??1????
		@str: ?????????
		@len: ?????????,????
*/
void u2_send_str(char* str,int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		u2_send_byte(str[i]);
	}
}

/*
	debug_init ??3(USART3)???
			@baudrate: ?????
*/
void usart3_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	/*
		1.??USART1???GPIO
				USART1_TX   PA9
				USART1_Rx	PA10
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin			=	GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode			=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd			=	GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	
	/*
		2.??USART
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						//???
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			//???8
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				//????
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				//???1
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	//???????
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	//????
	USART_Init(USART3, &USART_InitStruct);

	/*
		3.????(??????)
	*/
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	//???????????? ==>????????????

	/*
		4.??NVIC(????????NVIC)
	*/
	NVIC_InitStruct.NVIC_IRQChannel			=	USART3_IRQn;	//usart1?????
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/*
		5.????
	*/
	USART_Cmd(USART3,ENABLE);
}


unsigned char u3_recv[256] = {0};	/*????3??????*/
int u3_recv_count = 0;				/*????3????????*/
int u3_recv_flag = 0;				/*??????3???????? 1???? 0?????*/
void USART3_IRQHandler(void)
{
	static unsigned char ch;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		ch = USART_ReceiveData(USART3);		//???3 ????
		if(ch == 0xff)
		{
			u3_recv_count = 0;
		}
		u3_recv[u3_recv_count] = ch;
		u3_recv_count++;
		if(u3_recv_count == 9)
		{
			zmq = u3_recv[2]<<8 | u3_recv[3];
			u3_recv_count = 0;
			u3_recv_flag = 1;
		}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
}

/*
	u3_send_byte ??3??1???
		@ch: ??????????	
*/
void u3_send_byte(unsigned char ch)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) != SET);	//???????????
	USART_SendData(USART3,ch);
}
/*
	u3_send_str ??3??1????
		@str: ?????????
		@len: ?????????,????
*/
void u3_send_str(char* str,int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		u3_send_byte(str[i]);
	}
}


void gec_test_usart(void)
{
	printf("????2 ??? ??3\r\n");
	u2_send_str("TEST USART2\r\n",strlen("TEST USART2\r\n"));
	gec_delay_ms(2000);
	printf("????3 ??? ??2\r\n");
	u3_send_str("TEST USART3\r\n",strlen("TEST USART3\r\n"));
	gec_delay_ms(2000);
}
