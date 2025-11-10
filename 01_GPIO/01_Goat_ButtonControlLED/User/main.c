#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	//	初始化led引脚
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//	初始化按键引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	
	while(1)
	{
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
			delay_ms(10);
			if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
				GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
			}
		}else{
			GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
		}
	}
}
