#include "key.h"
#include "delay.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"

/********************
  *������ʼ���Ӻ���*
********************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*ʹ��GPIOAʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	/*��ʼ��GPIOA13��GPIOA15,��������*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	/*��ʼ��GPIOA0,��������*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
}

/***********************************************************************
  *����ɨ���Ӻ���*
  *mode=1��֧���������£�mode=0����֧����������*
  *����ֵ��KEY0���·���1,KEY1���·���2,WK_UP���·���3,�����κΰ�������0*
  *ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
************************************************************************/
u8 KEY_Scan(u8 mode)
{
	/*�������ֱ�־�����ڲ�֧����������ģʽ������0��ʱ���ʾ�Ѿ�����*/
	static u8 key_up=1;

	/*mode=1��ʱ��֧�ְ�����������*/
	if(mode)
		key_up=1;

	/*��ʼ����ɨ���ʱ��,����JTAG��SWD,��Ϊ��ʹ��GPIOA13��GPIOA15
	 *��Ϊ��ͨIO��ʹ��,STM32��Ĭ��״̬���ǿ���JTAG��*/
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(10);		//��ʱȥ��
		key_up=0;			//�������ֱ�־
		if(KEY0==0)			//KEY0����
		{
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,DISABLE);//����JTAG��SWD
			return RIGHT;	
		}
		else if(KEY1==0)	//KEY1����
		{
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,DISABLE);//����JTAG��SWD
			return MIDDLE;
		}
		else if(WK_UP==1)	//WK_UP����
		{
			GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,DISABLE);//����JTAG��SWD
			return LEFT;
		}	
	}
	else if(KEY0==1&&KEY1==1&&WK_UP==0)		//���ּ��
		key_up=1;
	
	return 0;								//�ް�������	
}
