#include "stm32f10x.h"                  // Device header
void My_USART_SendDatas(USART_TypeDef* USARTx, uint8_t *data, uint16_t size);
int main()
{
	// 初始化USART1引脚（非重映射）
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
	// 初始化USART1
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	
	uint8_t bytesSend[] = {1, 2, 3, 4, 5 };
	My_USART_SendDatas(USART1, bytesSend, 5);
	while(1)
	{
	}
}

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
