#include "stm32f10x.h"                  // Device header

/**
*@berif	LED的初始化函数 打开APB2外设使能 配置LED引脚此处为PA1和PA2为通用推挽输出 
*@param	无
*@retval	无
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

/**
*@berif	点亮LED1 此处LED1为低电平点亮
*@param	无
*@retval	无
*/
void LED1_On(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

/**
*@berif	关闭LED1 此处LED1为高电平熄灭
*@param	无
*@retval	无
*/
void LED1_Off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/**
*@berif	读取LED1此刻的电平，将其翻转
*@param	无
*@retval	无
*/
void LED1_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0){
		LED1_Off();
	}else{
		LED1_On();
	}
}

/**
*@berif	点亮LED2 此处LED2为低电平点亮
*@param	无
*@retval	无
*/
void LED2_On(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

/**
*@berif	关闭LED2 此处LED2为高电平熄灭
*@param	无
*@retval	无
*/
void LED2_Off(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}

/**
*@berif	读取LED2的电平 进行翻转
*@param	无
*@retval	无
*/
void LED2_Turn(void)
{
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0){
		LED2_Off();
	}else{
		LED2_On();
	}
}
