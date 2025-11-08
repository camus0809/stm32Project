#include "stm32f10x.h"                  // Device header

/**
*@berif		初始化光敏传感器 配置APB2的使能打开 PB13为上拉输入模式
*@param		无
*@retval	无
*/
void Sensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructrue;
	GPIO_InitStructrue.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructrue.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructrue.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructrue);
}

/**
*@berif		获取光敏传感器的值 光线过亮为1
*@param		无
*@retval	value 光敏传感器的值
*/
uint8_t Sensor_GetValue(void)
{
	uint8_t value = 0;
	value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
	return value;
}
