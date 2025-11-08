#include "stm32f10x.h"                  // Device header

/**
 *@berif
 *@param
 *@retval
 */ 
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;
	GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructrue);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2);
}

void LED1_On(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void LED1_Off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void LED1_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0){
		LED1_Off();
	}else{
		LED1_On();
	}
}
void LED2_On(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void LED2_Off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

void LED2_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0){
		LED2_Off();
	}else{
		LED2_On();
	}
}
