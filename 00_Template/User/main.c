#include "stm32f10x.h"                  // Device header

int main()
{
	// 寄存器使用
//	RCC->APB2ENR = 0x00000010;
//	GPIOC->CRH = 0x00300000;
//	GPIOC->ODR = 0x00002000; // PC13 = 1; 
	
	// 板载LED为PC13
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	// 与第6行作用相同 打开APB2的时钟使能
	GPIO_InitTypeDef GPIO_InitStructure;	// 定义一个结构体，由于初始化引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	// 通用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	// 选择13号引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 初始化MODE为输出模式，最大速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	// 为初始化PC13 
//	GPIO_SetBits(GPIOC, GPIO_Pin_13); // 设置GPIO_Pin_13为高电平
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);	// 设置GPIO_Pin_13为低电平
	while(1)
	{
		
	}
}
