#include "stm32f10x.h"                  // Device header
#include <stdio.h>
void My_USART1_Init(void);
void My_USART_SendDatas(USART_TypeDef* USARTx, uint8_t *data, uint16_t size);
int main()
{
	My_USART1_Init();
	
	printf("Hello world. \r\n");
	while(1)
	{
	}
}

/**
*@berif		通过USART1发送多个字节
*@param		USART1		串口名称
*@param		data		待发送的数据
*@param		size		发送数据的字节大小
*@retval	无
*/
void My_USART_SendDatas(USART_TypeDef* USARTx, uint8_t *data, uint16_t size)
{
	uint32_t i;
	for(i = 0; i < size; i++)
	{
		//	1. 确保发送寄存器均为0
		while(!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
		//	2. 写入发送寄存器
		USART_SendData(USARTx, *(data + i));
	}
	//	3. 等待发送完成
	while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));
}

/**
*@berif		初始化USART配置的初始化
			PA9 --> Tx	PA10 --> Rx		(非重映射）
			PB6 --> Tx	PB7  --> Rx		(重映射）
			波特率	115200
			数据位	8
			校验位	None
			停止位	1
*@param
*@retval
*/
void My_USART1_Init(void)
{
	// #1. 初始化USART1引脚（非重映射）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	// PA9 复用输出推挽
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// PA10 输入上拉
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	// 初始化USART1引脚（重映射）
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
//	
//	GPIO_InitTypeDef GPIO_InitStructure;
//	// PB6 复用输出推挽
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	// PB7 输入上拉
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	// #2. 初始化USART1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

int fputc(int ch, FILE *f)
{
	//	#1. 等待TDR为空
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	//	#2. 发送ch到发送寄存器中
	USART_SendData(USART1, (uint8_t)ch);
	
	return ch;
}
