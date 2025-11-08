#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"

uint8_t KeyNum;
//uint8_t LED1_Status = 1;	// 设置LED1的状态 1为灭 0为亮
//uint8_t LED2_Status = 1;	// 设置LED2的状态 1为灭 0为亮
int main()
{
	LED_Init();
	Key_Init();
	while(1)
	{
		KeyNum = Key_GetNum();
//		if(KeyNum == 1 && LED1_Status == 1){
//			LED1_Status = 0;
//			LED1_On();
//		}else if(KeyNum == 1 && LED1_Status == 0){
//			LED1_Status = 1;
//			LED1_Off();
//		}
//		
//		if(KeyNum == 2 && LED2_Status == 1){
//			LED2_Status = 0;
//			LED2_On();
//		}else if(KeyNum == 2 && LED2_Status == 0){
//			LED2_Status = 1;
//			LED2_Off();
//		}
		
		if(KeyNum == 1)
		{
			LED1_Turn();
		}else if(KeyNum == 2){
			LED2_Turn();
		}
	}
}
