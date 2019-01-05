#include "stellax_usart.h"
#include "ultrasonic.h"
#include "pwm.h"
#include "delay.h"

extern Speed speed_L,speed_R,control_L,control_R;

extern Distance dis[5];

char send_flag = 0, check_flag=0;

unsigned char test[6] = "stttte";
unsigned char lidar[6] = "slidae";
unsigned char fail[6] = "sfaile";
unsigned char temp[6] = {0};
unsigned char index_rx = 0;
unsigned char receive_flag = 0, operate_flag = 0;

	
void send_data(void)              //data = {'s',speed_L.c[0],speed_L.c[1],speed_R.c[0],speed_R.c[1],dis[0].c[0],dis[0].c[1],,,,,,,,,'e'}
{
	unsigned char data[16];
	int i;
	data[0] = 's';
	data[15] = 'e';
	if (send_flag){
   		for(i=0;i<2;i++){
			data[i+1] = speed_L.c[i];
			data[i+3] = speed_R.c[i];
		}
		for (i=0;i<2;i++){
			data[5+i] = dis[0].c[i];
			data[7+i] = dis[1].c[i];
			data[9+i] = dis[2].c[i];
			data[11+i] = dis[3].c[i];
			data[13+i] = dis[4].c[i];
		}
		//printf("USART sending data:\n");
		//printf("%d %d %d %d %d\n",dis[0].d,dis[1].d,dis[2].d,dis[3].d,dis[4].d);
	}
	if(check_flag){         //�Լ�
		for(i=0;i<6;i++){
			USART_SendData(USART1, test[i]);//�򴮿�1��������
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
  }
	else{                  //������������
			for(i=0;i<16;i++){
			USART_SendData(USART1, data[i]);//�򴮿�1��������
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
	}
}

void USART1_IRQHandler()
{
	char t;
  if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) 
  {  
    USART_ClearITPendingBit(USART1,USART_IT_RXNE); 
    temp[index_rx] = USART_ReceiveData(USART1);
		//�����յ�'s'��index_rxΪ0ʱ������������Ϊ֡ͷ�����ñ�־λ��ʼ��������
		if (temp[index_rx]=='s' && index_rx == 0)                             
		{
			receive_flag = 1;
//			printf("head\r\n");
		}
		//�����յ�'e'��index_rxΪ5ʱ������������Ϊ֡β�����ñ�־λ�������������ݣ���ʼУ������
		else if (temp[index_rx]=='e' && index_rx == 5)
		{
			receive_flag = 0;
			index_rx = 0;
			receive_flag = 0;
			operate_flag = 1;
			temp[0] = temp[5] = 0;
//			printf("tail\r\n");
		}
		//��index_rxΪ0�������յ������ֲ���'s'�����ߵ�index_rxΪ5�������յ����ݲ�Ϊ'e'���������մ��ң��������б�־λ�����½��ա�
		else if (check_flag == 1 && temp[index_rx]=='c'){
			check_flag = 0;
			index_rx = 0;
			GPIO_SetBits(GPIOC, GPIO_Pin_14);
//			printf("ok");
		}
		else if ( (index_rx==0 && temp[index_rx] != 's') || (index_rx==5 && temp[index_rx] != 'e')){
			index_rx = 0;
			receive_flag = 0;
			temp[0] = temp[5] = 0;
			operate_flag = 0;
//			printf("errer");
		}
		if (receive_flag == 1){
			index_rx++;
		}
		if (operate_flag==1){
			if(temp[1] == test[1] && temp[2] == test[2]  && temp[3] == test[3]  && temp[4] == test[4]){
				check_flag = 1;
		  	GPIO_ResetBits(GPIOC, GPIO_Pin_14);
		  }
			else if(temp[1] == lidar[1] && temp[2] == lidar[2]  && temp[3] == lidar[3]  && temp[4] == lidar[4]){
		  	GPIO_SetBits(GPIOB, GPIO_Pin_8);
		  }
			else if(temp[1] == fail[1] && temp[2] == fail[2]  && temp[3] == fail[3]  && temp[4] == fail[4]){
		  	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		  }
			else{
				for(t=0;t<2;t++){
				  control_L.c[t] = temp[1+t];
				  control_R.c[t] = temp[3+t];    //���ٶȸ�ֵ�������壬�ָ�float���͵��ٶ�
					temp[0] = temp[5] = 0;
				}
				//printf("left:%d right:%d",control_L.f,control_R.f);
		  }
			operate_flag = 0;
		}
    
  }

}
