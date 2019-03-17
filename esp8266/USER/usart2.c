/***************************************
 * 文件名  ：usart2.c
 * 描述    ：配置USART2         
 * 实验平台：基于STM32F103C8T6
 * 硬件连接：------------------------
 *          | PA2  - USART2(Tx)      |
 *          | PA3  - USART2(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  

**********************************************************************************/

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART2 时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* USART2 使用IO端口配置 */    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	  
	/* USART2 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;	//波特率设置：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART2, &USART_InitStructure);  //初始化USART2
	USART_Cmd(USART2, ENABLE);// USART2使能
}


/*发送一个字节数据*/
void UART2SendByte(unsigned char SendData)
{	   
	USART_SendData(USART2,SendData);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	    
}  


/*接收一个字节数据*/
unsigned char UART2GetByte(unsigned char* GetData)
{   	   
    if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
    {  
    	return 0;//没有收到数据 
	}
    *GetData = USART_ReceiveData(USART2); 
    return 1;//收到数据
}



void UART2Test(void)
{
	unsigned char i = 0;

	while(1)
	{    
		while(UART2GetByte(&i))
		{
		 	USART_SendData(USART2,i);
		}      
	}     
}


void Uart2_PutString(u8* buf)
{   
	u8 i;
    while(* buf)
    {
        Uart2_PutChar(*buf++);
    }
}


//发送一个字符
/*******************************************************************************
* Name  : Uart2_PutChar
* Deion : printf a char to the uart.
*******************************************************************************/
u8 Uart2_PutChar(u8 ch)
{
    /* Write a character to the USART */
   //      USART_ReceiveData           					  	//返回 USARTx 最近接收到的数据
   USART_SendData(USART2, (u8) ch);      //通过外设 USARTx 发送单个数据

   while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)		//等待数据发送完毕
   {		
   		//USART_GetFlagStatus    检查指定的 USART 标志位设置与否
  		//USART_FLAG_TXE     发送数据寄存器空标志位
   }
   return ch;
}


