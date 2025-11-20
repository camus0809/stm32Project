#include "stm32f10x.h"                  // Device header
#include "my_usart.h"
#include "OLED.h"
uint8_t RxData;
int main(void)
{
	My_USART_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "RxData: ");
	while(1)
	{
		if(My_USART_GetRxFlag() == 1)
		{
			RxData = My_USART_GetRxData();
			My_USART_SendByte(RxData);
			OLED_ShowHexNum(1, 8, RxData, 2);
		}
	}
}
