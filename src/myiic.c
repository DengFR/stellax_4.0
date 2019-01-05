#include <stm32f10x.h>
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "myiic.h"
 /*
u8 KS103_ReadOneByte(u8 address, u8 reg)
{				  
	u8 temp=0;		  	    																 
    IIC_Start();  
    IIC_Send_Byte(address);   //发送低地址
	IIC_Wait_Ack();	 
	IIC_Send_Byte(reg);   //发送低地址
	IIC_Wait_Ack();	   
	IIC_Start();  	 	   
	IIC_Send_Byte(address + 1);           //进入接收模式			   
	IIC_Wait_Ack();	 

	delay_us(50);	   //增加此代码通信成功！！！
    temp=IIC_Read_Byte(0);	  //读寄存器3	   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}


void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{				   	  	    																 
    IIC_Start();   
	IIC_Send_Byte(address);	    //发送写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);//发送高地址	  
	IIC_Wait_Ack();	   
    IIC_Send_Byte(command);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
    IIC_Stop();//产生一个停止条件 
}

*/

void IIC_Init(void)
{					     
 	RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 							 
	GPIOC->CRH&=0XFFF00FFF;//PC11/12 推挽输出
	GPIOC->CRH|=0X00033000;	   
	GPIOC->ODR|=3<<11;     //PC11,12 输出高
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(10);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(10);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(10);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(10);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(6);	   
	IIC_SCL=1;delay_us(6);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(10);
	IIC_SCL=1;
	delay_us(10);
	IIC_SCL=0;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(10);
	IIC_SCL=1;
	delay_us(10);
	IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(10);   
		IIC_SCL=1;
		delay_us(10); 
		IIC_SCL=0;	
		delay_us(10);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(10);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(5); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

/*
int main(void)
{		
	u16 range;
  	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);		//延时初始化
	uart_init(72,9600); //串口1初始化   
	while(1)
	{	  
			KS103_WriteOneByte(0XE8,0X02,0XB0);	  
  			delay_ms(80);
			range = KS103_ReadOneByte(0xe8, 0x02);
			range <<= 8;
			range += KS103_ReadOneByte(0xe8, 0x03);				   
	}
}

*/
























