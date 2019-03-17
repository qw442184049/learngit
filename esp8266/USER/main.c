/**************************************
 * �ļ���  ��main.c
 * ����    ��ͨ�����ں�ESP8266��������         
 * ʵ��ƽ̨��STM32F103C8T6
 * ��汾  ��ST3.0.0  																										  

*********************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "usart2.h"

void Delay(u32 nCount);
void esp8266_config(void);


int main(void)
{  
	     
	SystemInit();	//����ϵͳʱ��Ϊ 72M 

	USART1_Config(); //USART1 ���� 		
	USART2_Config();
	esp8266_config();

	while(1)
	{
		/*Delay(0xffffff);
		Uart1_PutString("AT+CIPSEND=0,6\r\n");  //ESP8266��������ʱ�����ȷ���һ��ATָ�����6�������ݳ���
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
	Uart1_PutString("AT+CWMODE=2\r\n");  //����ESP8266��ATָ��
	Delay(0xffffff);
	Uart1_PutString("AT+RST\r\n");
	Delay(0xffffff);	 
	Uart1_PutString("AT+CIPMUX=1\r\n");
	Delay(0xffffff);
	Uart1_PutString("AT+CIPSERVER=1,8080\r\n");
	Delay(0xffffff);
	Uart1_PutString("AT+CWSAP=\"123\",\"123456123456\",1,3\r\n" ); //�˴��ǽ�WIFI����Ϊ123��������123456123456
}


