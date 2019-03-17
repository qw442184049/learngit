/**************************************
 * 文件名  ：main.c
 * 描述    ：通过串口和ESP8266互传数据         
 * 实验平台：STM32F103C8T6
 * 库版本  ：ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "usart2.h"

void Delay(u32 nCount);
void esp8266_config(void);


int main(void)
{  
	     
	SystemInit();	//配置系统时钟为 72M 

	USART1_Config(); //USART1 配置 		
	USART2_Config();
	esp8266_config();

	while(1)
	{
		/*Delay(0xffffff);
		Uart1_PutString("AT+CIPSEND=0,6\r\n");  //ESP8266发送数据时必须先发这一条AT指令，其中6代表数据长度
		Delay(0xffffff);
		Uart1_PutString("ssssss");
		*/
	} 
}


void Delay(u32 nCount)
{ 
	for(; nCount != 0 ; nCount--);
}

void esp8266_config(void)
{ 
	Delay(0xffffff);
	Uart1_PutString("AT+CWMODE=2\r\n");  //配置ESP8266的AT指令
	Delay(0xffffff);
	Uart1_PutString("AT+RST\r\n");
	Delay(0xffffff);	 
	Uart1_PutString("AT+CIPMUX=1\r\n");
	Delay(0xffffff);
	Uart1_PutString("AT+CIPSERVER=1,8080\r\n");
	Delay(0xffffff);
	Uart1_PutString("AT+CWSAP=\"123\",\"123456123456\",1,3\r\n" ); //此处是将WIFI命名为123，密码是123456123456
}


