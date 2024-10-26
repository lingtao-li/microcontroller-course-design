

#ifndef __UART_H__
#define __UART_H__

/*
	debug_init ���Դ���(USART1)������
			@baudrate: ���ڲ�����
*/
void debug_init(int baudrate);

/*
	debug_send_byte ���Դ��ڷ���1���ֽ�
*/
void debug_send_byte(char ch);

/*
	debug_send_str ���Դ������һ���ַ���
*/
void debug_send_str(char* s,int len);

/*
	debug_init ����3(USART3)������
			@baudrate: ���ڲ�����
*/
void usart3_init(int baudrate);

/*
	debug_init ����2(USART2)������
			@baudrate: ���ڲ�����
*/
void usart2_init(int baudrate);

void u3_send_str(char* str,int len);

void u2_send_str(char* str,int len);

extern char recv_buf[512]; 	//������յ����ַ���
extern int  recv_len;	  	//�յ����ַ�������; 
extern int  recv_cnt;      	//��������
extern int  recv_flag;		  	// ������ɱ�־   		1�������    	0δ���

extern int  u2_recv_count;
extern int  u2_recv_flag;


extern int  u3_recv_count;
extern int  u3_recv_flag;

extern int zmq;
extern int lux;	
extern int pre,hum,height;

#endif
