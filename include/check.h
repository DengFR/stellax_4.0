#ifndef __CHECK_H
#define __CHECK_H

void Init_LED(void);
void Self_Checking(void);
#define Ultra_LED GPIO_Pin_13                //当超声波数据正常，LED点亮
#define Trans_LED GPIO_Pin_14                //当STM32与工控机通信正常，LED点亮
#define Moto_LED  GPIO_Pin_15                //当点击，编码器正常工作，LED点亮
#define STM_LED   GPIO_Pin_0                 //当STM32正常工作，LED点亮
#define Lidar_LED GPIO_Pin_8                 //当激光有数据返回时，LED点亮

#endif
