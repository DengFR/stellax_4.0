#ifndef __CHECK_H
#define __CHECK_H

void Init_LED(void);
void Self_Checking(void);
#define Ultra_LED GPIO_Pin_13                //������������������LED����
#define Trans_LED GPIO_Pin_14                //��STM32�빤�ػ�ͨ��������LED����
#define Moto_LED  GPIO_Pin_15                //�����������������������LED����
#define STM_LED   GPIO_Pin_0                 //��STM32����������LED����
#define Lidar_LED GPIO_Pin_8                 //�����������ݷ���ʱ��LED����

#endif
