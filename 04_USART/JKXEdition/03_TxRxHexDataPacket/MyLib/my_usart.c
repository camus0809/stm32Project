#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "stdarg.h"

uint8_t My_USART_TxPacket[4];
uint8_t My_USART_RxPacket[4];
uint8_t My_USART_RxFlag;
/**
*@berif		初始化USART1
*@param		无	
*@retval	无
*/
void My_USART_Init(void)
{
	//	#1. 开启USART1和GPIOA的时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//	#2. 初始化GPIO引脚
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	//	#3. 初始化USART配置
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	
	//	#5. 启动中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//·#6. 配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NIVC_InitStructure;
	NIVC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NIVC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NIVC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NIVC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NIVC_InitStructure);
	
	//	#4. 开启USART1
	USART_Cmd(USART1, ENABLE);
}

void My_USART_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	//	等待发送完成
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
}

void My_USART_SendBytes(uint8_t *Pbyte, uint16_t size)
{
	uint16_t i;
	for(i = 0; i < size; i++)
	{
		My_USART_SendByte(Pbyte[i]);
	}
}

void My_USART_SendString(char *string)
{
	uint8_t i = 0;
	while(string[i] != '\0'){
		while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
		My_USART_SendByte(string[i++]);
	}
	while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
}

void My_USART_SendNumber(uint32_t number)
{
    uint8_t digital_arr[11]; // 最多10位 + 结束符（也可以不需要）
    uint8_t i = 0;

    if (number == 0)
    {
        My_USART_SendByte('0');
        return;
    }

    // 取出各位（倒序）
    while (number > 0)
    {
        digital_arr[i++] = number % 10;
        number /= 10;
    }

    // 倒序发送（变成正序）
    while (i > 0)
    {
        i--;
        while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
        My_USART_SendByte(digital_arr[i] + '0'); // 转成 ASCII
    }

    while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
}

int fputc(int ch, FILE *f)
{
	My_USART_SendByte(ch);
	return ch;
}

void My_USART_Printf(char *format, ...)
{
	char string[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(string, format, arg);
	va_end(arg);
	My_USART_SendString(string);
}

void My_USART_SendPacket(void)
{
	My_USART_SendByte(0xFF);
	My_USART_SendBytes(My_USART_TxPacket,  4);
	My_USART_SendByte(0xFE); 
}

uint8_t My_USART_GetRxFlag(void)
{
	if(My_USART_RxFlag){
		My_USART_RxFlag = 0;
		return 1;
	}
	return 0;
}


void USART1_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t pRxPacket = 0;
	//	#1. 判断RDR中是否为空
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)){
		uint8_t RxData = USART_ReceiveData(USART1);
		if(RxState == 0){
			if(RxData == 0xFF){
				RxState = 1;
				pRxPacket = 0;
			}
		}else if(RxState == 1){
			My_USART_RxPacket[pRxPacket++] = RxData;
			if(pRxPacket >= 4){
				RxState = 2;
			}
		}else if(RxState == 2){
			 if(RxData == 0xFE){
				 RxState = 0;
				 My_USART_RxFlag = 1;
			 }
		}
		
		USART_ClearITPendingBit(USART1, USART_FLAG_RXNE);
	}
}
