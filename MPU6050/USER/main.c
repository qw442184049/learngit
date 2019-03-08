#include "usart.h" //串口头文件
#include "sys.h" //系统头文件
#include "delay.h" //延时
#include "mpu6050.h" //陀螺仪
#include "stdio.h" //标准c
#include "stm32f10x.h" //stm32相关
#include "math.h" //数学函数

void key_config(void); //按键初始化函数

void Tim3_Init(void);  //定时器3初始化函数，用来输出pwm
void Tim2_Init(void);  //定时器2初始化函数，用来输出pwm

int m1 = 0, m2 = 0, m3 = 0, m4 = 0; //pwm数值初始化

void main(void)
{
	delay_init();	 //初始化延时
	uart_init(115200); //初始化串口

	delay_ms(1500); //系统延时
	MPU6050_Init(); //初始化陀螺仪
    key_config(); //初始化按键
	SystemInit(); //系统初始化
    Tim3_Init(); //初始化定时器3
	Tim2_Init(); //初始化定时器2
      
    TIM_SetCompare1(TIM3, 0); //定时器3通道1初始值为0
    TIM_SetCompare2(TIM3, 0); //定时器3通道2初始值为0
  
	TIM_SetCompare3(TIM2, 0); //定时器2通道3初始值为0
    TIM_SetCompare4(TIM2, 0); //定时器2通道4初始值为0
	
  	//做寻迹车所以陀螺仪部分注释掉
	
	while(1) //死循环
	{
    	MPU6050_Pose(); //陀螺仪姿态解算
     
		/*下面的所有printf函数接上串口线，就是之前让你们买的那个usb转ttl那个小模块，
		可以直接在电脑上任意的串口助手软件上看到结果，你们用一个角度就够了，下面的
		三个角度自己选一个用，这个和你陀螺仪的安装位置有关，尽量水平着装陀螺仪*/
		
	  	printf("角度%f\n", Roll); //输出横滚角大小
	  	printf("加速度x%d\n", xgyro); 
		/*输出轴加速度，这个和底下的几个加速度与上面的角度未必一一对应
    	，不过不重要，你们应该用不上轴加速度*/
   
		printf("角度%f\n", Pitch); //输出旋转角大小
		printf("加速度y%d\n", ygyro);
		
	  	printf("角度%f\n", Yaw); //输出俯仰角大小
		printf("加速度z%d\n", zgyro);

		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0) /*这个是预留的一个按键，接口是PA12，如果IO口输入为低电平*/
		{
			printf("button push");//输出button push
		}
    	else 
    	{
	      	printf("button off"); //输出button off
    	}
  
		/*底下的四个函数是用来设置pwm值的大小的，你们给m1m2m3m4赋值多少，pwm输出的占空比就是m/2000，所以值不能大于
		1999，1999已经是满输出了，相当于直接把电机接到了电源上，一般来说是车上会有四个电机，但只需要俩组驱动，
		即一个L298N模块足以使用，每两组pwm控制一个驱动，所以共启用了四个pwm口，这样实现的是H桥可以正反转的功能，
		举例来说，你将L298N的输出线（随意正负）接到电机上，将信号线（就是pwm口的线）接到L298N输入口，此时速度
		大小为m1-m2的值，方向由值的正负决定
		*/
		
		TIM_SetCompare1(TIM3, m1); //第一组
	    TIM_SetCompare2(TIM3, m2); //H桥pwm
	
        TIM_SetCompare3(TIM2, m3); //第二组
        TIM_SetCompare4(TIM2, m4); //H桥pwm	 
		
		/*上面都看完就能想怎么编程了，举例来说，我取陀螺仪roll角，如果角度大于30度，我希望车子
		正走，角度小于-30度，我希望车子倒走，大于-30且小于30，车子不动，则函数要这样写
		if(roll > 30)
		{
		    TIM_SetCompare1(TIM3, 1500); //第一组
	      	TIM_SetCompare2(TIM3, 500); //H桥pwm
	
        	TIM_SetCompare3(TIM2, 1500); //第二组
        	TIM_SetCompare4(TIM2, 500);	//H桥pwm	 
		}
		else if(roll < -30)
		{
		    TIM_SetCompare1(TIM3, 500); //第一组
	      	TIM_SetCompare2(TIM3, 1500); //H桥pwm
	
        	TIM_SetCompare3(TIM2, 500); //第二组
        	TIM_SetCompare4(TIM2, 1500);	//H桥pwm	 
		}
		else if(roll > -30 && roll < 30)
		{
		    TIM_SetCompare1(TIM3, 0); //第一组
	      	TIM_SetCompare2(TIM3, 0); //H桥pwm
	
        	TIM_SetCompare3(TIM2, 0); //第二组
        	TIM_SetCompare4(TIM2, 0); //H桥pwm	 
		}
		你只需要改变m1m2m3m4的值就行了，不想用变值，就直接设定好定值, 以后用pid肯定是要用变值的
		*/
   }
}


//这一部分是配置各种寄存器的
//有兴趣的研究下，F103的主频是72M的，底下的配置是实现100HZ的pwm输出的
//自己找找底下的数字是怎么实现100HZ方波的
void Tim3_Init(void) //定时器三
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

void Tim2_Init(void) //定时器二
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


void key_config(void)//按键配置
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //开启按键端口PB的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //开启按键端口PA的时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //端口配置为上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化端口

	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; 
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //端口配置为上拉输入
	//GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化端口
}

