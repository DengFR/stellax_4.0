#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "wheel.h"
#include "timer.h"
#include "param.h"
#include "pwm.h"
#include "stellax_usart.h"
#include "ultrasonic.h"
#include "24cxx.h"
#include "check.h"

int main(void)
 { 
		uart_init(115200);
	  delay_init();
		Init_Control();              //初始化PWM模块
    Init_Encoder();              //初始化编码器接收模块
	  Init_Ultra(400);             //初始化超声波接收模块  
    Init_LED();                  //初始化指示灯
	 
	  Self_Checking();             //开机自检
	  
    while(1)
	   {
			  Ultra_Check();           //程序运行时不断检查超声波
				Get_Speed();             //从编码器读取里程计数据，超声波数据自动定时读取
				send_data();             //每50ms发送超声波和里程计数据给上层
			  Set_Speed();             //设置移动平台速度
				
		 }			 
}
