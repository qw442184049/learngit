#include "led.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

/********************
  *LED��ʼ���Ӻ���*
********************/
void LED_Init(void)					
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*ʹ��GPIOA��GPIODʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);

	/*��ʼ��GPIOA8,����ٶ�50MHz��ͨ���������*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*GPIOA8����ߵ�ƽ,GPIOA8��Ӧ��LED0*/
	GPIO_SetBits(GPIOA,GPIO_Pin_8);

	/*��ʼ��GPIOD2,����ٶ�50MHz��ͨ���������*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	/*GPIOD2����ߵ�ƽ,GPIOD2��Ӧ��LED1*/
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
}
