

#ifndef __UART_H__
#define __UART_H__

/*
	debug_init 调试串口(USART1)的配置
			@baudrate: 串口波特率
*/
void debug_init(int baudrate);

/*
	debug_send_byte 调试串口发送1个字节
*/
void debug_send_byte(char ch);

/*
	debug_send_str 调试串口输出一个字符串
*/
void debug_send_str(char* s,int len);

/*
	debug_init 串口3(USART3)的配置
			@baudrate: 串口波特率
*/
void usart3_init(int baudrate);

/*
	debug_init 串口2(USART2)的配置
			@baudrate: 串口波特率
*/
void usart2_init(int baudrate);

void u3_send_str(char* str,int len);

void u2_send_str(char* str,int len);

extern char recv_buf[512]; 	//保存接收到的字符串
extern int  recv_len;	  	//收到的字符串长度; 
extern int  recv_cnt;      	//接收数量
extern int  recv_flag;		  	// 接收完成标志   		1接收完成    	0未完成

extern int  u2_recv_count;
extern int  u2_recv_flag;


extern int  u3_recv_count;
extern int  u3_recv_flag;

extern int zmq;
extern int lux;	
extern int pre,hum,height;

#endif
