#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/**
*@berif	初始化按键 开启APB2的时钟使能 配置PB1 PB11为输入上拉模式
*@param	无
*@retval	无
*/
void Key_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;
	GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructrue);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_2);
}
 
/**
*@berif		获取按键的键值 KEY1按下为1 KEY2按下为2
*@param		无
*@retval	keyNum 按键的键值
*/
uint8_t Key_GetNum(void)
{
	uint8_t keyNum = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)){ // 按键按下
		Delay_ms(20);
		while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1));
		Delay_ms(20);
		
		keyNum = 1;
	}
	
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)) {	// 按键按下
		Delay_ms(20);
		while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11));
		Delay_ms(20);
		
		keyNum = 2;
	}
	
	return keyNum;
}	 	 
