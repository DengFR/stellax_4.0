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
    IIC_Send_Byte(address);   //���͵͵�ַ
	IIC_Wait_Ack();	 
	IIC_Send_Byte(reg);   //���͵͵�ַ
	IIC_Wait_Ack();	   
	IIC_Start();  	 	   
	IIC_Send_Byte(address + 1);           //�������ģʽ			   
	IIC_Wait_Ack();	 

	delay_us(50);	   //���Ӵ˴���ͨ�ųɹ�������
    temp=IIC_Read_Byte(0);	  //���Ĵ���3	   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}


void KS103_WriteOneByte(u8 address,u8 reg,u8 command)
{				   	  	    																 
    IIC_Start();   
	IIC_Send_Byte(address);	    //����д����
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);//���͸ߵ�ַ	  
	IIC_Wait_Ack();	   
    IIC_Send_Byte(command);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
    IIC_Stop();//����һ��ֹͣ���� 
}

*/

void IIC_Init(void)
{					     
 	RCC->APB2ENR|=1<<4;//��ʹ������IO PORTCʱ�� 							 
	GPIOC->CRH&=0XFFF00FFF;//PC11/12 �������
	GPIOC->CRH|=0X00033000;	   
	GPIOC->ODR|=3<<11;     //PC11,12 �����
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(10);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(10);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(10);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(10);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
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
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}

/*
int main(void)
{		
	u16 range;
  	Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);		//��ʱ��ʼ��
	uart_init(72,9600); //����1��ʼ��   
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
























