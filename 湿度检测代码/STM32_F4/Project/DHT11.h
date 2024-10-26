#ifndef __DHT11_H
#define __DHT11_H

#include <stm32f4xx.h>

void DHT11_init(void);

uint8_t DHT11_read(uint8_t * humi,uint8_t * humi_frac,uint8_t *temp,uint8_t *temp_frac);
#endif 
