#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Buzzer.h"
#include "Sensor.h"

int main()
{
	Buzzer_Init();
	Sensor_Init();
	while(1)
	{
		
		if(Sensor_GetValue() == 0){
			Buzzer_Off();
		}else{
			Buzzer_On();
		}
	}
}
