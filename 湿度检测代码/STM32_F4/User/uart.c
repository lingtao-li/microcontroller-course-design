
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "gec_delay.h"

int zmq;
int lux;	
int pre,hum,height;
extern uint8_t humi;

/*
	debug_init ���Դ���(USART1)������
			@baudrate: ���ڲ�����
	����ֵ��
			��
*/
void debug_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	/*
		1.����USART1��Ӧ��GPIO
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
		2.����USART
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						//������
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			//����λ8
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				//��ҪУ��
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				//ֹͣλ1
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	//��ҪӲ��������
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStruct);

	/*
		3.ʹ���ж�(�����Ҫ�ж�)
	*/
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//�������ݼĴ�����Ϊ���ж� ==>�����ݿ��Խ���ʱ�����ж�

	/*
		4.����NVIC(ֻҪ�����жϾ�ҪNVIC)
	*/
	NVIC_InitStruct.NVIC_IRQChannel			=	USART1_IRQn;	//usart1�ж�������
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/*
		5.��������
	*/
	USART_Cmd(USART1,ENABLE);

}

/*
	debug_send_byte ���Դ��ڷ���1���ֽ�
		@ch�� Ҫ���͵��ֽ�����
*/
void debug_send_byte(int ch)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) != SET);	//�ȴ��������ݼĴ���Ϊ��
	
	USART_SendData(USART1,ch);									//�Ӵ��ڷ���1���ֽ�
}

/*
	debug_send_str ���Դ������һ���ַ���
		@s�� ָ��Ҫ���͵��ַ���
		@len: Ҫ���͵��ֽ���
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
	�����ж�,��Ҫ������������
	������յ���������Ҫ����,���������һ��  "ͨ��Э��"
	"ͨ��Э��": ͨ��˫��Լ��,��һ���ַ���ʲô?
							  �� ���һ���ַ���ʲô?
							  �� �������ַ�?
	����:
		Լ��: �� \r\n ��β
*/
char recv_buf[512] = {0}; 	//������յ����ַ���
int  recv_len   = 0;	  	//�յ����ַ�������; 
int  recv_cnt	= 0;      	//��������
int  recv_flag = 0;		  	// ������ɱ�־   		1�������    	0δ���
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
//			ch1= USART_ReceiveData(USART1);		//�Ӵ���1 ��������
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
//		recv_buf[recv_cnt]	=	ch;			//���յ������ݱ���
//		if(recv_buf[recv_cnt-1]=='\r'&&ch == '\n' || recv_cnt==510)     //������ղ�����
//		{//�������
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
//    //test2: ���MCU���յ��� PC�˵����ݣ���תLED��״̬
//	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
//	{
//		ch = USART_ReceiveData(USART1);		//�Ӵ���1 ��������
//        //.....
//		if(ch == 'ON')
//        {
//            GPIO_ResetBits(GPIOF,GPIO_Pin_10);
//	GPIO_ResetBits(GPIOF,GPIO_Pin_9);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_13);
//	GPIO_ResetBits(GPIOE,GPIO_Pin_14);
//        }
//        //��������жϱ�־
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//	}
//}



/*
	debug_init ����2(USART2)������
			@baudrate: ���ڲ�����
*/
void usart2_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	/*
		1.����USART2��Ӧ��GPIO
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin			=	GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode			=	GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd			=	GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	/*
		2.����USART
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						//������
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			//����λ8
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				//��ҪУ��
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				//ֹͣλ1
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	//��ҪӲ��������
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStruct);

	/*
		3.ʹ���ж�(�����Ҫ�ж�)
	*/
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//�������ݼĴ�����Ϊ���ж� ==>�����ݿ��Խ���ʱ�����ж�

	/*
		4.����NVIC(ֻҪ�����жϾ�ҪNVIC)
	*/
	NVIC_InitStruct.NVIC_IRQChannel			=	USART2_IRQn;	//usart1�ж�������
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/*
		5.��������
	*/
	USART_Cmd(USART2,ENABLE);
}



unsigned char u2_recv[256] = {0};	/*�洢����2���յ�������*/
int u2_recv_count = 0;				/*��¼����2���յ������ݳ���*/
int u2_recv_flag = 0;				/*���ڱ�ʶ����2�����Ƿ������� 1��ʾ��� 0��ʾδ���*/
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

	
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)//��ȡ�����жϱ�־
	{
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//		ch = USART_ReceiveData(USART2);		//�Ӵ���1 ��������
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
		
		
		
		
//		else if(u2_recv[2]==0x45 &&  u2_recv_count==14)//��ʪ��
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
	u2_send_byte ����2����1���ֽ�
*/
void u2_send_byte(unsigned char ch)
{
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) != SET);	//�ȴ��������ݼĴ���Ϊ��
	USART_SendData(USART2,ch);
}

/*
	u2_send_str ����2����1���ַ���
		@str�� ָ��Ҫ���͵��ַ���
		@len�� Ҫ�����ַ����ĳ��ȣ���λ�ֽ�
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
	debug_init ����3(USART3)������
			@baudrate: ���ڲ�����
*/
void usart3_init(int baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	/*
		1.����USART1��Ӧ��GPIO
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
		2.����USART
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	USART_InitStruct.USART_BaudRate				=	baudrate;						//������
	USART_InitStruct.USART_WordLength			=	USART_WordLength_8b;			//����λ8
	USART_InitStruct.USART_Parity				=	USART_Parity_No;				//��ҪУ��
	USART_InitStruct.USART_StopBits				=	USART_StopBits_1;				//ֹͣλ1
	USART_InitStruct.USART_HardwareFlowControl	=	USART_HardwareFlowControl_None;	//��ҪӲ��������
	USART_InitStruct.USART_Mode					=	USART_Mode_Tx | USART_Mode_Rx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStruct);

	/*
		3.ʹ���ж�(�����Ҫ�ж�)
	*/
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	//�������ݼĴ�����Ϊ���ж� ==>�����ݿ��Խ���ʱ�����ж�

	/*
		4.����NVIC(ֻҪ�����жϾ�ҪNVIC)
	*/
	NVIC_InitStruct.NVIC_IRQChannel			=	USART3_IRQn;	//usart1�ж�������
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	=	2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 	2;
	NVIC_InitStruct.NVIC_IRQChannelCmd		=	ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/*
		5.��������
	*/
	USART_Cmd(USART3,ENABLE);
}


unsigned char u3_recv[256] = {0};	/*�洢����3���յ�������*/
int u3_recv_count = 0;				/*��¼����3���յ������ݳ���*/
int u3_recv_flag = 0;				/*���ڱ�ʶ����3�����Ƿ������� 1��ʾ��� 0��ʾδ���*/
void USART3_IRQHandler(void)
{
	static unsigned char ch;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		ch = USART_ReceiveData(USART3);		//�Ӵ���3 ��������
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
	u3_send_byte ����3����1���ֽ�
		@ch�� Ҫ���͵��Ǹ��ֽ�����	
*/
void u3_send_byte(unsigned char ch)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) != SET);	//�ȴ��������ݼĴ���Ϊ��
	USART_SendData(USART3,ch);
}
/*
	u3_send_str ����3����1���ַ���
		@str�� ָ��Ҫ���͵��ַ���
		@len�� Ҫ�����ַ����ĳ��ȣ���λ�ֽ�
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
	printf("���Դ���2 ���͸� ����3\r\n");
	u2_send_str("TEST USART2\r\n",strlen("TEST USART2\r\n"));
	gec_delay_ms(2000);
	printf("���Դ���3 ���͸� ����2\r\n");
	u3_send_str("TEST USART3\r\n",strlen("TEST USART3\r\n"));
	gec_delay_ms(2000);
}
