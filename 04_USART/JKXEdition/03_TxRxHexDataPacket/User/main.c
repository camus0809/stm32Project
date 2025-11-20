#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "my_usart.h"
#include "key.h"

uint8_t keyNumber;
uint8_t i;
int main(void)
{
	OLED_Init();
	My_Key_Init();
	My_USART_Init();
	OLED_ShowString(1, 1, "TxPacket:");
	OLED_ShowString(3, 1, "RxPacket:");
	My_USART_TxPacket[0] = 0x01;
	My_USART_TxPacket[1] = 0x02;
	My_USART_TxPacket[2] = 0x03;
	My_USART_TxPacket[3] = 0x04;
	
	My_USART_SendPacket();
	while(1)
	{
		// #1. 发送数据包
		keyNumber = My_Key_Scan();
		if(keyNumber == 1){
			My_USART_TxPacket[0]++;
			My_USART_TxPacket[1]++;
			My_USART_TxPacket[2]++;
			My_USART_TxPacket[3]++;
			
			My_USART_SendPacket();
			OLED_ShowHexNum(2, 1, My_USART_TxPacket[0], 2);
			OLED_ShowHexNum(2, 4, My_USART_TxPacket[1], 2);
			OLED_ShowHexNum(2, 7, My_USART_TxPacket[2], 2);
			OLED_ShowHexNum(2, 10, My_USART_TxPacket[3], 2);
		}
		if(My_USART_GetRxFlag() == 1){
			OLED_ShowHexNum(4, 1, My_USART_RxPacket[0], 2); 
			OLED_ShowHexNum(4, 4, My_USART_RxPacket[1], 2); 
			OLED_ShowHexNum(4, 7, My_USART_RxPacket[2], 2); 
			OLED_ShowHexNum(4, 10, My_USART_RxPacket[3], 2); 
		}
	}
}
