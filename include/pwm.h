#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
#include "timer.h"

typedef union
{
	short int f;
	unsigned char c[2];
}Speed;

void TIM8_PWM_Init(u16 arr,u16 psc);
void Set_Speed(void);
void Init_Control(void);
void Direct_Init(void);
void control_left(int speed);
void control_right(int speed);
void Moto_Check(void);
void delay_ms_(u16 i);

#endif
