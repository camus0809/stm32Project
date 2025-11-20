#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "my_usart.h"
int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "Hello world");
	
	My_USART_Init();
	
	My_USART_Printf("你好，世界");
	while(1)
	{
	}
}
