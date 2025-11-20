#include "stm32f10x.h"                  // Device header

void My_LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void My_LED_On(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
}

void My_LED_Off(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
}
