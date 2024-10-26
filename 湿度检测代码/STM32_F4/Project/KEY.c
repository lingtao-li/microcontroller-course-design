
#include "stm32f4xx.h"
#include "key.h"
#include "gec_delay.h"
void key_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}
int key1_detect()
{
	if(key1==0)
	{
		gec_delay_ms(20);
			if(key1==0)
			{

          //gec_delay_ms(20);
				while(key1==0);
				return 1;	
			//如果为1，说明按键按下
				gec_delay_ms(20);
			}
			else
			{
				gec_delay_ms(20);
				//while(key1==0);
		    return 0;//如果为0，说明按键未按下
		  gec_delay_ms(20);
			}
	}
	else
		{
		return 0;
	  
		}
}

int key2_detect()
{
	if(key2==0)
	{
		gec_delay_ms(20);
			if(key2==0)
			{

          //gec_delay_ms(20);
				while(key2==0);
				return 1;	
			//如果为1，说明按键按下
				gec_delay_ms(20);
			}
			else
			{
				gec_delay_ms(20);
				//while(key1==0);
		    return 0;//如果为0，说明按键未按下
		  gec_delay_ms(20);
			}
	}
	else
		{
		return 0;
	  
		}
}
int key3_detect()
{
	if(key3==0)
	{
		gec_delay_ms(20);
			if(key3==0)
			{

          //gec_delay_ms(20);
				while(key3==0);
				return 1;	
			//如果为1，说明按键按下
				gec_delay_ms(20);
			}
			else
			{
				gec_delay_ms(20);
				//while(key1==0);
		    return 0;//如果为0，说明按键未按下
		  gec_delay_ms(20);
			}
	}
	else
		{
		return 0;
	  
		}
}
int key4_detect()
{
	if(key4==0)
	{
		gec_delay_ms(20);
			if(key4==0)
			{

          //gec_delay_ms(20);
				while(key4==0);
				return 1;	
			//如果为1，说明按键按下
				gec_delay_ms(20);
			}
			else
			{
				gec_delay_ms(20);
				//while(key1==0);
		    return 0;//如果为0，说明按键未按下
		  gec_delay_ms(20);
			}
	}
	else
		{
		return 0;
	  
		}
}
