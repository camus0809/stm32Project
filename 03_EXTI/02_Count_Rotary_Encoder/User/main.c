#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Encoder.h"

int16_t Num;
int main(void)
{	
	OLED_Init();
	Encoder_Init();
	OLED_ShowString(1, 1, "count:");
	while(1)
	{
		Num += Encoder_GetValue();
		OLED_ShowSignedNum(1, 7, Num, 3);
	}
}
