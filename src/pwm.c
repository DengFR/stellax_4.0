#include "pwm.h"
#include "param.h"
#include "math.h"
#include "delay.h"
#include "stdlib.h"



Speed control_L,control_R,speed_L,speed_R;


void Init_Control(void)
{
	TIM8_PWM_Init(899,0);
	Direct_Init();
	control_L.f = 0;
	control_R.f = 0;
}

void Direct_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设时钟使能
	                                                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_SetBits(GPIOA,GPIO_Pin_3);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
}

void Stop(void)
{}

void control_left(int speed)
{
	if(speed > 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		TIM_SetCompare1(TIM8,356+abs(speed)*convert);
	}
	else if (speed < 0){
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		TIM_SetCompare1(TIM8,356+abs(speed)*convert);
	}
	else{
		TIM_SetCompare1(TIM8,0);
	}
}
	
void control_right(int speed)
{
	if(speed > 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		TIM_SetCompare2(TIM8,340+abs(speed)*convert);
	}
	else if (speed < 0){
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		TIM_SetCompare2(TIM8,340+abs(speed)*convert);
	}
	else {
		TIM_SetCompare2(TIM8,0);
	}
}
	
/*利用定时器1，配置*/
void TIM8_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能GPIO外设时钟使能
	                                                                     	

   //设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //TIM_CH1，TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx（通道一）
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx （通道二）

  TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能	

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能	 
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
	
 
}

void Set_Speed(void)
{
		control_left(control_L.f);
		control_right(control_R.f);  	
}

void Moto_Check(void)
{
	 control_left(350);
	 control_right(-350);
	 Get_Speed();
	 delay_ms_(200);
	 if (speed_L.f != 0 && speed_R.f != 0){
		 GPIO_SetBits(GPIOC,GPIO_Pin_15);
	 }	
	 control_left(0);
	 control_right(0);
	 delay_ms_(200);

	 control_left(-350);
	 control_right(350);
   Get_Speed();
	 delay_ms_(200);
	 if(speed_L.f != 0 && speed_R.f != 0){
		 GPIO_SetBits(GPIOC,GPIO_Pin_15);
	 }
}

void delay_ms_(u16 i)
{
	int a,b,c;
	for(a=0;a<i;a++)
	{
		for(b=0;b<1000;b++)
		{
			for(c=0;c<8;c++);
		}
	}
}
