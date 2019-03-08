#ifndef __KEY_H__
#define __KEY_H__
#include "stm32f10x.h"

#define KEY0 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_13)	//KEY0��Ӧ��GPIOA13 
#define KEY1 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)	//KEY1��Ӧ��GPIOA15
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) 	//WK_UP��Ӧ��GPIOA0

#define RIGHT 	1
#define MIDDLE	2
#define LEFT 	3

void KEY_Init(void);		//������ʼ���Ӻ���
u8 KEY_Scan(u8 mode);		//����ɨ���Ӻ���

#endif
