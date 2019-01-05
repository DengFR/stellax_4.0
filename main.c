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
		Init_Control();              //��ʼ��PWMģ��
    Init_Encoder();              //��ʼ������������ģ��
	  Init_Ultra(400);             //��ʼ������������ģ��  
    Init_LED();                  //��ʼ��ָʾ��
	 
	  Self_Checking();             //�����Լ�
	  
    while(1)
	   {
			  Ultra_Check();           //��������ʱ���ϼ�鳬����
				Get_Speed();             //�ӱ�������ȡ��̼����ݣ������������Զ���ʱ��ȡ
				send_data();             //ÿ50ms���ͳ���������̼����ݸ��ϲ�
			  Set_Speed();             //�����ƶ�ƽ̨�ٶ�
				
		 }			 
}
