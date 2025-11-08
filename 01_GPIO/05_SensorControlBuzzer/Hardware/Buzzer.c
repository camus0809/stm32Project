#include "stm32f10x.h"                  // Device header

/**
*@berif		初始化蜂鸣器 打开APB2时钟使能 配置PB12为推挽输出模式
*@param		无
*@retval	无
*/
void Buzzer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;
	GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructrue);
}

/**
*@berif		使蜂鸣器鸣叫	
*@param		无	
*@retval	无
*/
void Buzzer_On(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

/**
*@berif		使蜂鸣器停止鸣叫
*@param		无
*@retval	无
*/
void Buzzer_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
