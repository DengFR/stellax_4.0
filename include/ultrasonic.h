#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H
#include "sys.h"

void Get_distance(u16 *d);
void Init_Ultra(u16 t);
void Release(void);
void print_distance(void);
void Ultra_Check(void);

typedef union
{
		u16 d;
	  unsigned char c[2];
}Distance;

#endif
