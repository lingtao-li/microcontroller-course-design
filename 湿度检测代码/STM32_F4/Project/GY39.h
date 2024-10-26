#ifndef __GY39_H
#define __GY39_H

void debug_init(int baudrate);


void debug_send_byte(char ch);


void debug_send_str(char* s,int len);


void usart3_init(int baudrate);

void usart2_init(int baudrate);

void u3_send_str(char* str,int len);

void u2_send_str(char* str,int len);

extern char recv_buf[512]; 
extern int  recv_len;	  	
extern int  recv_cnt;      	
extern int  recv_flag;		  	

extern int  u2_recv_count;
extern int  u2_recv_flag;


extern int  u3_recv_count;
extern int  u3_recv_flag;

extern int zmq;
extern int lux;	
extern int temp,pre,hum,height;

#endif
