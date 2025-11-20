#include "stm32f10x.h"                  // Device header
void My_USART1_Init(void);
void My_OnBoardLED_Init(void);
int main(void)
{
	My_USART1_Init();
	My_OnBoardLED_Init();
//	printf("Hello world. \r\n");
	while(1)
	{
		//	#1. 等待RDR非空
		while(!USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
		//	#2. 读取RDR中的数据
		uint16_t RDRValue = USART_ReceiveData(USART1);
		//	#3. 根据RDR中的数据对板载的LED进行对应的操作
		if(RDRValue == '0')
		{
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		}else if(RDRValue== '1'){
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		}
	}
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

void My_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
}
