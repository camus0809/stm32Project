#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "my_usart.h"
#include "Delay.h"
#include "LED.h"
#include <string.h>

uint8_t keyNumber;
uint8_t i;
int main(void)
{
	My_USART_Init();
	Delay_ms(10);//	OLED_Init() 内的大延时（软件空循环）导致 USART 初始化失效
	OLED_Init();
	My_LED_Init();
	
	OLED_ShowString(1, 1, "TxPacket:");
	OLED_ShowString(3, 1, "RxPacket:");
	while(1)
	{
		if(My_USART_GetRxFlag() == 1){
			OLED_ShowString(4, 1, "                ");
			OLED_ShowString(4, 1, My_USART_RxPacket);
			
			if(strcmp(My_USART_RxPacket, "LED_ON") == 0){
			My_LED_On();
			My_USART_SendString("LED_ON_OK\r\n");
			
			OLED_ShowString(2, 1, "                ");
			OLED_ShowString(2, 1, "LED_ON_OK");
			}else if(strcmp(My_USART_RxPacket, "LED_OFF") == 0){
				My_LED_Off();
				My_USART_SendString("LED_OFF_OK\r\n");
				
				OLED_ShowString(2, 1, "                "); 
				OLED_ShowString(2, 1, "LED_OFF_OK");
			}else{
				My_USART_SendString("ERROR_COMMAND\r\n");
				OLED_ShowString(2, 1, "                ");
				OLED_ShowString(2, 1, "ERROR_COMMAND");
			}
		}
		
		
	}
}
