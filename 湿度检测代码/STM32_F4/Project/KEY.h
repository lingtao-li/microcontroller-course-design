#ifndef __KEY_H
#define __KEY_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#define key1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define key2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define key3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)
#define key4 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)
void key_Init(void);
int key1_detect();
int key2_detect();
int key3_detect();
int key4_detect();
#endif 
