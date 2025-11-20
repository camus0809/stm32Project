#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "OnBoardLED.h"

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
