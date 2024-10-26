#ifndef _CAR_H
#define _CAR_H
#define D0 GPIO_Pin_6
#define D1 GPIO_Pin_7
#define D2 GPIO_Pin_8
#define D3 GPIO_Pin_9
#define WHL GPIOC
void CarInit(void);
void Car_test(int,int);
void Car_PWM(int n);
void Car_Forward(int i);
void Car_Left(int i);
void Car_Right(int i);
#endif
