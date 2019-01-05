#include "delay_.h"

void delay_us(u16 i)
{
	int a,b;
	for(a=0; a<i; a++){
		for(b=0;b<8;b++);
	}
}
void delay_ms(u16 i)
{
	int a;
	for(a=0; a<i; a++){
		delay_us(1000);
	}
}
