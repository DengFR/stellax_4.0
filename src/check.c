#include "stm32f10x.h"
#include "check.h"
#include "delay.h"
#include "pwm.h"
#include "ultrasonic.h"

void delay_ms_(u16 i);

void Init_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //使能GPIO外设时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_0 ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC, Ultra_LED);
	GPIO_ResetBits(GPIOC, Trans_LED);
	GPIO_ResetBits(GPIOC, Moto_LED);
	GPIO_ResetBits(GPIOC, STM_LED);
	GPIO_ResetBits(GPIOB, Lidar_LED);
}


void LED_Check(void)
{
	int i;
	for (i=0;i<3;i++){
		GPIO_SetBits(GPIOC, Ultra_LED);
		GPIO_SetBits(GPIOC, Trans_LED);
		GPIO_SetBits(GPIOC, Moto_LED);
		GPIO_SetBits(GPIOC, STM_LED);
		GPIO_SetBits(GPIOB, Lidar_LED);
		delay_ms_(500);
		GPIO_ResetBits(GPIOC, Ultra_LED);
		GPIO_ResetBits(GPIOC, Trans_LED);
		GPIO_ResetBits(GPIOC, Moto_LED);
		GPIO_ResetBits(GPIOC, STM_LED);
		GPIO_ResetBits(GPIOB, Lidar_LED);
		delay_ms_(500);
	}
}

void Self_Checking(void)
{
	LED_Check();
	Ultra_Check();
	Moto_Check();
	GPIO_SetBits(GPIOC, STM_LED);
}

//void delay_ms_(u16 i)
//{
//	int a,b,c;
//	for(a=0;a<i;a++)
//	{
//		for(b=0;b<1000;b++)
//		{
//			for(c=0;c<8;c++);
//		}
//	}
//}
