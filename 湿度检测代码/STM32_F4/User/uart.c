
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "gec_delay.h"

int zmq;
int lux;	
int pre,hum,height;
extern uint8_t humi;

/*
	debug_init 调试串口(USART1)的配置
			@baudrate: 串口波特率
	返回值：
			无
*/
void debug_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	/*
		1.配置USART1对应的GPIO
				USART1_TX   PA9
				USART1_Rx	PA10
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin			=	GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode			=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd			=	GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	/*
		2.配置USART
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						//波特率
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			//数据位8
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				//不要校验
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				//停止位1
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	//不要硬件控制流
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	//收发模式
	USART_Init(USART1, &USART_InitStruct);

	/*
		3.使能中断(如果需要中断)
	*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//接收数据寄存器不为空中断 ==>有数据可以接收时触发中断

	/*
		4.配置NVIC(只要用了中断就要NVIC)
	*/
	NVIC_InitStruct.NVIC_IRQChannel			=	USART1_IRQn;	//usart1中断请求线
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/*
		5.开启串口
	*/
	USART_Cmd(USART1,ENABLE);

}

/*
	debug_send_byte 调试串口发送1个字节
		@ch： 要发送的字节数据
*/
void debug_send_byte(int ch)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) != SET);	//等待发送数据寄存器为空
	
	USART_SendData(USART1,ch);									//从串口发送1个字节
}

/*
	debug_send_str 调试串口输出一个字符串
		@s： 指向要发送的字符串
		@len: 要发送的字节数
*/
void debug_send_str(char* s,int len)
{
	int i =0;
	for(i=0;i<len;i++)
	{
		debug_send_byte(s[i]);
	}
}


/*
	串口中断,主要用来接收数据
	如果您收到的数据需要保存,那最好是有一个  "通信协议"
	"通信协议": 通信双发约定,第一个字符发什么?
							  或 最后一个字符是什么?
							  或 发多少字符?
	比如:
		约定: 以 \r\n 结尾
*/
char recv_buf[512] = {0}; 	//保存接收到的字符串
int  recv_len   = 0;	  	//收到的字符串长度; 
int  recv_cnt	= 0;      	//接收数量
int  recv_flag = 0;		  	// 接收完成标志   		1接收完成    	0未完成
int ii=0;
//unsigned char buf[10] = {0};
//	unsigned char ch1;
//	unsigned char ch2;
//	unsigned char ch3;
//int ccc=0;
void USART1_IRQHandler(void)

{

	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
//		ii++;
//		if(ii==1)
//		{
//			ch1= USART_ReceiveData(USART1);		//从串口1 接收数据
//		}else if(ii==2){
//			ch2= USART_ReceiveData(USART1);	
//		}else if(ii==3){
//			ch3= USART_ReceiveData(USART1);
//		}
		
		
		ii= USART_ReceiveData(USART1);
	//	ii++;
		//buf[i]=ch;
		//i++;
		//debug_send_byte(ch);
//		recv_buf[recv_cnt]	=	ch;			//将收到的数据保存
//		if(recv_buf[recv_cnt-1]=='\r'&&ch == '\n' || recv_cnt==510)     //收完或收不下了
//		{//接收完成
//			recv_flag = 1;
//			recv_len  = recv_cnt;
//			recv_cnt  = 0;
//		}
//		else
//		{
//			recv_cnt++;
//		}
//if(buf[ii-1]=='#'){
//	ii= 0;
//		if(buf[0]=='O'&&buf[1]=='N'){
//	GPIO_ResetBits(GPIOF,GPIO_Pin_10);
//	GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_13);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
//			
//			
//			
//		}else if(buf[0]=='O'&&buf[1]=='F'&&buf[2]=='F'){
//	GPIO_SetBits(GPIOF,GPIO_Pin_10);
//	GPIO_SetBits(GPIOF,GPIO_Pin_9);
//	GPIO_SetBits(GPIOE,GPIO_Pin_13);
//	GPIO_SetBits(GPIOE,GPIO_Pin_14);
//			
//		}else{
//			
//			
//			GPIO_SetBits(GPIOF,GPIO_Pin_8);
//			
//						//GPIO_ResetBits(GPIOE,GPIO_Pin_2);
////			if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_8)==1){
////			gec_delay_ms(1000);
//			///ccc=1;
////			GPIO_ResetBits(GPIOF,GPIO_Pin_8);
////			
////			
////		}
////			gec_delay_ms(1000);
////			GPIO_ResetBits(GPIOF,GPIO_Pin_8);
//			//ii= 0;
//		}
//	}
		
//		if(ch1=='O'&&ch2=='N'){
//	GPIO_ResetBits(GPIOF,GPIO_Pin_10);
//	GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_13);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
//			ii= 0;
//			//USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//			
//		}else if(ch1=='O'&&ch2=='F'){
//	GPIO_SetBits(GPIOF,GPIO_Pin_10);
//	GPIO_SetBits(GPIOF,GPIO_Pin_9);
//	GPIO_SetBits(GPIOE,GPIO_Pin_13);
//	GPIO_SetBits(GPIOE,GPIO_Pin_14);
//			ii= 0;
//		}else{
//			
//			
//			//GPIO_SetBits(GPIOF,GPIO_Pin_8);
//			
//			//gec_delay_ms(1000);
//			GPIO_ResetBits(GPIOF,GPIO_Pin_8);
//			//ii= 0;
//		}
//		
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
//void USART1_IRQHandler(void)
//{
//	unsigned char ch;
//    //test2: 如果MCU接收到了 PC端的数据，翻转LED灯状态
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
//	{
//		ch = USART_ReceiveData(USART1);		//从串口1 接收数据
//        //.....
//		if(ch == 'ON')
//        {
//            GPIO_ResetBits(GPIOF,GPIO_Pin_10);
//	GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_13);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
//        }
//        //清除串口中断标志
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//	}
//}



/*
	debug_init 串口2(USART2)的配置
			@baudrate: 串口波特率
*/
void usart2_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	/*
		1.配置USART2对应的GPIO
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin			=	GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode			=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd			=	GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	/*
		2.配置USART
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						//波特率
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			//数据位8
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				//不要校验
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				//停止位1
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	//不要硬件控制流
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	//收发模式
	USART_Init(USART2, &USART_InitStruct);

	/*
		3.使能中断(如果需要中断)
	*/
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//接收数据寄存器不为空中断 ==>有数据可以接收时触发中断

	/*
		4.配置NVIC(只要用了中断就要NVIC)
	*/
	NVIC_InitStruct.NVIC_IRQChannel			=	USART2_IRQn;	//usart1中断请求线
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/*
		5.开启串口
	*/
	USART_Cmd(USART2,ENABLE);
}



unsigned char u2_recv[256] = {0};	/*存储串口2接收到的数据*/
int u2_recv_count = 0;				/*记录串口2接收到的数据长度*/
int u2_recv_flag = 0;				/*用于标识串口2数据是否接收完成 1表示完成 0表示未完成*/
int llt1;
int llt2;
int llt3;
int llt4;
int llt5;
int llt6;

uint8_t low;

uint8_t hig;
int iii=0;
char buf[10] = {0};
void USART2_IRQHandler(void)
{

	
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)//获取串口中断标志
	{
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		ch = USART_ReceiveData(USART2);		//从串口1 接收数据
//		u2_recv[u2_recv_count] = ch;
//		u2_recv_count++;
//		if(u2_recv[2]==0x15 && u2_recv_count==8)
//		{
//			lux = u2_recv[4]<<24 | u2_recv[5]<<16 | u2_recv[6]<<8 | u2_recv[7];
//			u2_recv_count = 0;
//			u2_recv_flag = 1;
//		iii++;
//		if(iii==1){
//			llt1=USART_ReceiveData(USART2);
//		}else if(iii==2){
//			llt2=USART_ReceiveData(USART2);
//		}
////		}else if(iii==3){
////			llt3=USART_ReceiveData(USART2);
////		}else if(iii==4){
////			llt4=USART_ReceiveData(USART2);
////		}else if(iii==5){
////			llt5=USART_ReceiveData(USART2);
////		}else if(iii==6){
////			llt6=USART_ReceiveData(USART2);
////		}
////			
//		if(iii==2){
//						//if(humi<llt1||humi>llt2){
//			debug_send_byte(llt1);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
//							
//										debug_send_byte(llt2);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
//							
//										debug_send_byte(llt3);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
//							
//							
//										debug_send_byte(llt4);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
//							
//										debug_send_byte(llt5);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
////							
//			
////			
////			llt1=llt1-'0';
////			llt2=llt2-'0';
////			llt3=llt3-'0';
////			llt4=llt4-'0';
////			llt5=llt5-'0';
//////			llt6=llt6-'0';
////			low=llt1*10+llt2;
////			hig=llt3*10+llt4;
////			
////													debug_send_byte(low);
////	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
////														debug_send_byte(hig);
////	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
////				GPIO_SetBits(GPIOF,GPIO_Pin_8);
////	gec_delay_ms(1000);
////				
////				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
//							iii=0;
//		//	}
//			
//		}
//		
//		

		buf[iii]=USART_ReceiveData(USART2);
		iii++;
//		if(buf[iii-1]=='#'){
//			llt1=buf[0]*10+buf[1];
//			llt2=buf[3]*10+buf[4];
//			if(humi<llt1||humi>llt2){
//				//GPIO_SetBits(GPIOF,GPIO_Pin_8);
//			//gec_delay_ms(1000);
//				
//				GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//							
//			}else{
//				//GPIO_ToggleBits(GPIOF ,GPIO_Pin_8); 
//				//GPIO_ResetBits(GPIOF,GPIO_Pin_8);
//				
//				GPIO_SetBits(GPIOF,GPIO_Pin_9);
//			}
//			
//			
//			iii=0;
//			}
if(buf[iii-1]=='#'){
	
	debug_send_byte(buf[0]);
	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
	
	debug_send_byte(buf[1]);//1
	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
	
	debug_send_byte(buf[2]);//0
	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
	
	debug_send_byte(buf[3]);
	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
	
	debug_send_byte(buf[4]);//2
	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
//	
//	debug_send_byte(buf[5]);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
//	
//	debug_send_byte(buf[6]);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
	
//		llt1=buf[0]-'0';
//		llt2=buf[1]-'0';
//		llt3=buf[2]-'0';
//		llt4=buf[3]-'0';
//		llt5=buf[4]-'0';
//					
//					
//			low=llt1*10+llt2;
//			hig=llt4*10+llt5;
//			
//			debug_send_byte(low);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
//	
//	debug_send_byte(hig);
//	debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
//	
//			if(humi<low||humi>hig){
//				//GPIO_SetBits(GPIOF,GPIO_Pin_8);
//			//gec_delay_ms(1000);
//				
//				GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//							
//			}else{
////				GPIO_ToggleBits(GPIOF ,GPIO_Pin_8); 
////				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
//				
//				GPIO_SetBits(GPIOF,GPIO_Pin_9);
//			}
//	
//	iii=0;
}
				
		}
		
		
		
		
//		else if(u2_recv[2]==0x45 &&  u2_recv_count==14)//温湿度
//		{
//			temp = u2_recv[4]<<8 | u2_recv[5];
//			pre = u2_recv[6]<<24 | u2_recv[7]<<16 | u2_recv[8]<<8 | u2_recv[9];
//			hum = u2_recv[10]<<8 | u2_recv[11];
//			height = u2_recv[12]<<8 | u2_recv[13];
//			u2_recv_count = 0;
//			u2_recv_flag = 1;
//		}		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}

/*
	u2_send_byte 串口2发送1个字节
*/
void u2_send_byte(unsigned char ch)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);	//等待发送数据寄存器为空
	USART_SendData(USART2,ch);
}

/*
	u2_send_str 串口2发送1个字符串
		@str： 指向要发送的字符串
		@len： 要发送字符串的长度，单位字节
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
	debug_init 串口3(USART3)的配置
			@baudrate: 串口波特率
*/
void usart3_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	/*
		1.配置USART1对应的GPIO
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
		2.配置USART
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						//波特率
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			//数据位8
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				//不要校验
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				//停止位1
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	//不要硬件控制流
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	//收发模式
	USART_Init(USART3, &USART_InitStruct);

	/*
		3.使能中断(如果需要中断)
	*/
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	//接收数据寄存器不为空中断 ==>有数据可以接收时触发中断

	/*
		4.配置NVIC(只要用了中断就要NVIC)
	*/
	NVIC_InitStruct.NVIC_IRQChannel			=	USART3_IRQn;	//usart1中断请求线
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/*
		5.开启串口
	*/
	USART_Cmd(USART3,ENABLE);
}


unsigned char u3_recv[256] = {0};	/*存储串口3接收到的数据*/
int u3_recv_count = 0;				/*记录串口3接收到的数据长度*/
int u3_recv_flag = 0;				/*用于标识串口3数据是否接收完成 1表示完成 0表示未完成*/
void USART3_IRQHandler(void)
{
	static unsigned char ch;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		ch = USART_ReceiveData(USART3);		//从串口3 接收数据
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
	u3_send_byte 串口3发送1个字节
		@ch： 要发送的那个字节数据	
*/
void u3_send_byte(unsigned char ch)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) != SET);	//等待发送数据寄存器为空
	USART_SendData(USART3,ch);
}
/*
	u3_send_str 串口3发送1个字符串
		@str： 指向要发送的字符串
		@len： 要发送字符串的长度，单位字节
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
	printf("测试串口2 发送给 串口3\r\n");
	u2_send_str("TEST USART2\r\n",strlen("TEST USART2\r\n"));
	gec_delay_ms(2000);
	printf("测试串口3 发送给 串口2\r\n");
	u3_send_str("TEST USART3\r\n",strlen("TEST USART3\r\n"));
	gec_delay_ms(2000);
}
