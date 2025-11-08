#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main()
{	
	OLED_Init();
	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "hello");
	OLED_ShowNum(2, 1, 12345, 5);
	OLED_ShowSignedNum(2, 7, -66, 2);
	OLED_ShowHexNum(3, 1, 0xAAAA, 4);
	OLED_ShowBinNum(4, 1, 0xAAAA, 16);
	
//	OLED_Clear();
	while(1)
	{
	}
}
