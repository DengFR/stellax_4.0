#include <stm32f10x.h>
#include "ultrasonic.h"
#include "usart.h"		
#include "delay.h"	
#include "24cxx.h"

Distance dis[5];


u8 add[5] = {0xd0,0xd2,0xd4,0xd6,0xd8};
u8 i = 0;

void Release()
{
	int j = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
  GPIO_Init(GPIOC, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
	
	if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)==0)
	{
		  for(j = 0; j < 10; j++) 			
			{
				GPIO_ResetBits(GPIOC, GPIO_Pin_12);
				delay_us(100);
				if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)!=0)
				{
					break;
				}
				GPIO_SetBits(GPIOC, GPIO_Pin_12);
				delay_us(100);
			}
			if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)!=0 && j>=10)
			{
				return;
			}
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
			GPIO_ResetBits(GPIOC, GPIO_Pin_11);
			delay_us(100);
			GPIO_SetBits(GPIOC, GPIO_Pin_12);
			delay_us(100);
			GPIO_SetBits(GPIOC, GPIO_Pin_11);
			delay_us(100);
	}
}

void TIM5_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到500为50ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM5, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设
							 
}

void TIM5_IRQHandler(void)   //TIM3中断
{
	u16 range = 0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		range = KS103_ReadOneByte(add[i], 0x02);
		range <<= 8;
		range += KS103_ReadOneByte(add[i], 0x03);
		dis[i].d = range;
		i++;
		if(i==5){
			i = 0;
		}
		KS103_WriteOneByte(add[i],0X02,0Xb0);
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
} 

void Init_Ultra(u16 t)           //t表示超声波传感器更新的时间，建议不低于50ms
{
	char b;
	//Release(); 
	TIM5_Init(t-1,7199);
	AT24CXX_Init();
	for (b=0;b<5;b++)
	{
		dis[b].d = 0;
	}
	KS103_WriteOneByte(add[0],0X02,0Xb0);
	delay_ms(t);

}

void print_distance(void){
	printf("%d %d %d %d %d\n",dis[0].d,dis[1].d,dis[2].d,dis[3].d,dis[4].d);
}

void Ultra_Check(void)
{
	if (dis[0].d > 0 && dis[0].d < 6000 &&
		  dis[1].d > 0 && dis[1].d < 6000 &&
	    dis[2].d > 0 && dis[2].d < 6000 &&
	    dis[3].d > 0 && dis[2].d < 6000 &&
	    dis[4].d > 0 && dis[2].d < 6000)
	{
	    GPIO_SetBits(GPIOC,GPIO_Pin_13);		
	}
	else{
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
}
//void Get_distance(u16 *d)
//{
//	u16 range,range1,range2,range3,range4; 
//	KS103_WriteOneByte(0Xd0,0X02,0Xb0);
//	delay_ms(80); 
//	range = KS103_ReadOneByte(0xd0, 0x02);
//	range <<= 8;
//	range += KS103_ReadOneByte(0xd0, 0x03);
//	d[0]=range;
//	KS103_WriteOneByte(0Xd2,0X02,0Xb0);
//	delay_ms(80); 
//	range1 = KS103_ReadOneByte(0xd2, 0x02);
//	range1 <<= 8;
//	range1 += KS103_ReadOneByte(0xd2, 0x03);
//	d[1]=range1;
//	KS103_WriteOneByte(0Xd4,0X02,0Xb0);
//	delay_ms(80); 
//	range2 = KS103_ReadOneByte(0xd4, 0x02);
//	range2 <<= 8;
//	range2 += KS103_ReadOneByte(0xd4, 0x03);
//	d[2]=range2;
//	KS103_WriteOneByte(0Xd6,0X02,0Xb0);
//	delay_ms(80); 
//	range3 = KS103_ReadOneByte(0xd6, 0x02);
//	range3 <<= 8;
//	range3 += KS103_ReadOneByte(0xd6, 0x03);
//	d[3]=range3;
//	KS103_WriteOneByte(0Xd8,0X02,0Xb0);
//	delay_ms(80); 
//	range4 = KS103_ReadOneByte(0xd8, 0x02);
//	range4 <<= 8;
//	range4 += KS103_ReadOneByte(0xd8, 0x03);
//	d[4]=range4;
//}
