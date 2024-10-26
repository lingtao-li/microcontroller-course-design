#include "main.h"

#include "gec_delay.h"
#include "LED.h"
#include "DHT11.h"
#include <stdio.h>
#include "uart.h"
int fputc(int c,FILE* fp)
{
	debug_send_byte(c&0xff);

	return c;
}


	uint8_t humi,temp,fin;
	uint8_t humi_frac,temp_frac;
int main()
{
//	uint8_t humi,temp,fin;
//	uint8_t humi_frac,temp_frac;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	debug_init(9600);
	usart2_init(9600);
	DHT11_init();
	key_Init();
	led();
//		
	GPIO_SetBits(GPIOF,GPIO_Pin_10);
	GPIO_SetBits(GPIOF,GPIO_Pin_9);
	GPIO_SetBits(GPIOE,GPIO_Pin_13);
	GPIO_SetBits(GPIOE,GPIO_Pin_14);
	while(1)
	{
		DHT11_read(&humi,&humi_frac,&temp,&temp_frac);
		//printf("\tnext\t \n");
		gec_delay_ms(500);

	printf("\t humi %d.%d ",humi,humi_frac);
				fin=humi;
		
		
		
		//debug_send_str("hello PC\r\n",strlen("hello PC\r\n"));
		u2_send_byte(humi);
		
		
		//GPIO_ResetBits(GPIOE,GPIO_Pin_14);
		if(key1_detect()==1){
			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
			//fin=humi+humi_frac/10;
			if(fin<60||fin>70){
				GPIO_SetBits(GPIOF,GPIO_Pin_8);
	gec_delay_ms(1000);
				
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			}
			
		}
		else if(key2_detect()==1){
			
			 GPIO_ToggleBits(GPIOF ,GPIO_Pin_10); 
				if(fin<70||fin>80){
				GPIO_SetBits(GPIOF,GPIO_Pin_8);
				gec_delay_ms(1000);
				
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			}
			
		}
		else if(key3_detect()==1){
			GPIO_ToggleBits(GPIOE,GPIO_Pin_13);
					if(fin<80||fin>90){
				GPIO_SetBits(GPIOF,GPIO_Pin_8);
	gec_delay_ms(1000);
				
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			}
			
		}
		else if(key4_detect()==1){
			GPIO_ToggleBits(GPIOE,GPIO_Pin_14);
					if(fin<90||fin>100){
				GPIO_SetBits(GPIOF,GPIO_Pin_8);
	gec_delay_ms(1000);
				
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			}
			
		}
		
		
	}
}
//void USART1_IRQHandler(void)
//{

//	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
//	{
//		if(0 == USART_ReceiveData(USART1))
//		{
//						 Lit_led(1,0);
//				     Lit_led(2,0);
//						 Lit_led(3,0);
//						 Lit_led(4,0);
//		}
//		else if(1 == USART_ReceiveData(USART1))
//		{
//						 Lit_led(1,1);
//				     Lit_led(2,1);
//						 Lit_led(3,1);
//						 Lit_led(4,1);
//		}

//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
//	}
//}

