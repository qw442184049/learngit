#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void UART1Test(void);
void Uart1_PutString(u8* buf);
u8 Uart1_PutChar(u8 ch);


#endif /* __USART1_H */
