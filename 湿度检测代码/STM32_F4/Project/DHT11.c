#include "dht11.h"
#include "gec_delay.h"

void DHT11_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;  
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}





static uint8_t DHT11_read_bitdat(void)
{
	uint8_t i;
	uint8_t dat;
	
	
	for(i=0;i<8;i++)
	{	
		dat <<= 1;  
		
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)) ; 
		
		while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0);  
		
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)) 
		{
			gec_delay_us(30);
		}
		
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
			dat |= 1; 
				
	}
	
	return dat;
}


uint8_t DHT11_read(uint8_t * humi,uint8_t * humi_frac,uint8_t *temp,uint8_t *temp_frac)
{
	uint8_t retry = 0;
	uint8_t i;
	uint8_t buf[5];
	
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	gec_delay_ms(20);  
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	gec_delay_us(30);  
	
		
	while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6))
	{
		gec_delay_us(2);
		retry++;		
		if(retry > 10)
		{	
			return 1; 
		}
	}
	retry = 0;
	while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0)
	{
		gec_delay_us(10);
		retry++;		
		if(retry > 10)
		{	
			return 1; 
		}
	}

	for(i=0;i<5;i++)
	{
		buf[i] = DHT11_read_bitdat();
	}
	
	
	if(buf[0]+buf[1]+buf[2]+buf[3] == buf[4])
	{
		*humi = buf[0];
		*humi_frac = buf[1];
		*temp = buf[2];
		*temp_frac = buf[3];
		return 0; 
	}
	
	return 1;
}

