#include "usart.h" //����ͷ�ļ�
#include "sys.h" //ϵͳͷ�ļ�
#include "delay.h" //��ʱ
#include "mpu6050.h" //������
#include "stdio.h" //��׼c
#include "stm32f10x.h" //stm32���
#include "math.h" //��ѧ����

void key_config(void); //������ʼ������

void Tim3_Init(void);  //��ʱ��3��ʼ���������������pwm
void Tim2_Init(void);  //��ʱ��2��ʼ���������������pwm

int m1 = 0, m2 = 0, m3 = 0, m4 = 0; //pwm��ֵ��ʼ��

void main(void)
{
	delay_init();	 //��ʼ����ʱ
	uart_init(115200); //��ʼ������

	delay_ms(1500); //ϵͳ��ʱ
	MPU6050_Init(); //��ʼ��������
    key_config(); //��ʼ������
	SystemInit(); //ϵͳ��ʼ��
    Tim3_Init(); //��ʼ����ʱ��3
	Tim2_Init(); //��ʼ����ʱ��2
      
    TIM_SetCompare1(TIM3, 0); //��ʱ��3ͨ��1��ʼֵΪ0
    TIM_SetCompare2(TIM3, 0); //��ʱ��3ͨ��2��ʼֵΪ0
  
	TIM_SetCompare3(TIM2, 0); //��ʱ��2ͨ��3��ʼֵΪ0
    TIM_SetCompare4(TIM2, 0); //��ʱ��2ͨ��4��ʼֵΪ0
	
  	//��Ѱ�������������ǲ���ע�͵�
	
	while(1) //��ѭ��
	{
    	MPU6050_Pose(); //��������̬����
     
		/*���������printf�������ϴ����ߣ�����֮ǰ����������Ǹ�usbתttl�Ǹ�Сģ�飬
		����ֱ���ڵ���������Ĵ�����������Ͽ��������������һ���ǶȾ͹��ˣ������
		�����Ƕ��Լ�ѡһ���ã�������������ǵİ�װλ���йأ�����ˮƽ��װ������*/
		
	  	printf("�Ƕ�%f\n", Roll); //�������Ǵ�С
	  	printf("���ٶ�x%d\n", xgyro); 
		/*�������ٶȣ�����͵��µļ������ٶ�������ĽǶ�δ��һһ��Ӧ
    	����������Ҫ������Ӧ���ò�������ٶ�*/
   
		printf("�Ƕ�%f\n", Pitch); //�����ת�Ǵ�С
		printf("���ٶ�y%d\n", ygyro);
		
	  	printf("�Ƕ�%f\n", Yaw); //��������Ǵ�С
		printf("���ٶ�z%d\n", zgyro);

		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0) /*�����Ԥ����һ���������ӿ���PA12�����IO������Ϊ�͵�ƽ*/
		{
			printf("button push");//���button push
		}
    	else 
    	{
	      	printf("button off"); //���button off
    	}
  
		/*���µ��ĸ���������������pwmֵ�Ĵ�С�ģ����Ǹ�m1m2m3m4��ֵ���٣�pwm�����ռ�ձȾ���m/2000������ֵ���ܴ���
		1999��1999�Ѿ���������ˣ��൱��ֱ�Ӱѵ���ӵ��˵�Դ�ϣ�һ����˵�ǳ��ϻ����ĸ��������ֻ��Ҫ����������
		��һ��L298Nģ������ʹ�ã�ÿ����pwm����һ�����������Թ��������ĸ�pwm�ڣ�����ʵ�ֵ���H�ſ�������ת�Ĺ��ܣ�
		������˵���㽫L298N������ߣ������������ӵ�����ϣ����ź��ߣ�����pwm�ڵ��ߣ��ӵ�L298N����ڣ���ʱ�ٶ�
		��СΪm1-m2��ֵ��������ֵ����������
		*/
		
		TIM_SetCompare1(TIM3, m1); //��һ��
	    TIM_SetCompare2(TIM3, m2); //H��pwm
	
        TIM_SetCompare3(TIM2, m3); //�ڶ���
        TIM_SetCompare4(TIM2, m4); //H��pwm	 
		
		/*���涼�����������ô����ˣ�������˵����ȡ������roll�ǣ�����Ƕȴ���30�ȣ���ϣ������
		���ߣ��Ƕ�С��-30�ȣ���ϣ�����ӵ��ߣ�����-30��С��30�����Ӳ���������Ҫ����д
		if(roll > 30)
		{
		    TIM_SetCompare1(TIM3, 1500); //��һ��
	      	TIM_SetCompare2(TIM3, 500); //H��pwm
	
        	TIM_SetCompare3(TIM2, 1500); //�ڶ���
        	TIM_SetCompare4(TIM2, 500);	//H��pwm	 
		}
		else if(roll < -30)
		{
		    TIM_SetCompare1(TIM3, 500); //��һ��
	      	TIM_SetCompare2(TIM3, 1500); //H��pwm
	
        	TIM_SetCompare3(TIM2, 500); //�ڶ���
        	TIM_SetCompare4(TIM2, 1500);	//H��pwm	 
		}
		else if(roll > -30 && roll < 30)
		{
		    TIM_SetCompare1(TIM3, 0); //��һ��
	      	TIM_SetCompare2(TIM3, 0); //H��pwm
	
        	TIM_SetCompare3(TIM2, 0); //�ڶ���
        	TIM_SetCompare4(TIM2, 0); //H��pwm	 
		}
		��ֻ��Ҫ�ı�m1m2m3m4��ֵ�����ˣ������ñ�ֵ����ֱ���趨�ö�ֵ, �Ժ���pid�϶���Ҫ�ñ�ֵ��
		*/
   }
}


//��һ���������ø��ּĴ�����
//����Ȥ���о��£�F103����Ƶ��72M�ģ����µ�������ʵ��100HZ��pwm�����
//�Լ����ҵ��µ���������ôʵ��100HZ������
void Tim3_Init(void) //��ʱ����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitdata;	
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
			
	TIM_TimeBaseInitdata.TIM_Prescaler = 359;	
	TIM_TimeBaseInitdata.TIM_ClockDivision = 0x00;	
	TIM_TimeBaseInitdata.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInitdata.TIM_Period = 1999;	
	TIM_TimeBaseInitdata.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitdata);
	TIM_ARRPreloadConfig(TIM3, ENABLE);	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
	TIM_OCInitStructure.TIM_Pulse = 0x1fff;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	

	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;	
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;	

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	
	TIM_OC1FastConfig(TIM3, TIM_OCFast_Enable); 
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2FastConfig(TIM3, TIM_OCFast_Enable); 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 

	TIM_ITConfig(TIM3,TIM_IT_Update, DISABLE); 
	TIM_ClearFlag(TIM3, TIM_FLAG_Update); 
	TIM_Cmd(TIM3, ENABLE); 
}

void Tim2_Init(void) //��ʱ����
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitdata;	
	TIM_OCInitTypeDef TIM_OCInitStructure;	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);	
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		
	TIM_TimeBaseInitdata.TIM_Prescaler = 359;	
	TIM_TimeBaseInitdata.TIM_ClockDivision = 0x00;	
	TIM_TimeBaseInitdata.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInitdata.TIM_Period = 1999;	
	TIM_TimeBaseInitdata.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitdata);
	TIM_ARRPreloadConfig(TIM2, ENABLE);	

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	
	TIM_OCInitStructure.TIM_Pulse = 0x1fff;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	

	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;	
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;	
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;	

	TIM_OC3Init(TIM2, &TIM_OCInitStructure);	
	TIM_OC3FastConfig(TIM2, TIM_OCFast_Enable); 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 

	TIM_OC4Init(TIM2, &TIM_OCInitStructure);	
	TIM_OC4FastConfig(TIM2, TIM_OCFast_Enable); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); 

	TIM_ITConfig(TIM2, TIM_IT_Update,DISABLE); 
	TIM_ClearFlag(TIM2, TIM_FLAG_Update); 
	TIM_Cmd(TIM2, ENABLE); 
}


void key_config(void)//��������
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //���������˿�PB��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //���������˿�PA��ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //�˿�����Ϊ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ���˿�

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; 
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //�˿�����Ϊ��������
	//GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ���˿�
}

