#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "InfraredSensor.h"

uint16_t Num;
int main()
{	
	OLED_Init();
	InfraredSensor_Init();
	OLED_ShowString(1, 1, "Num:");
	while(1)
	{
		OLED_ShowNum(1, 5, Num,3);
		Num = InfraredSensor_GetValue();
	}
}
