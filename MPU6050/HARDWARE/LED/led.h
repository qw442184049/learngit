#ifndef __LED_H__
#define __LED_H__
#include "sys.h"

#define LED0 PAout(8)	//LED0��Ӧ��GPIOA8
#define LED1 PDout(2)	//LED1��Ӧ��GPIOD2

void LED_Init(void);	//LED��ʼ���Ӻ���

#endif
