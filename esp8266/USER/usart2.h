#ifndef __USART2_H
#define	__USART2_H

#include "stm32f10x.h"
#include <stdio.h>

void USART2_Config(void);
void UART2Test(void);
void Uart2_PutString(u8* buf);
u8 Uart2_PutChar(u8 ch);


#endif /* __USART1_H */

