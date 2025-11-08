#include "stm32f10x.h"                  // Device header

int16_t Cycle_Count;

/**
*@berif		初始化旋转编码器的A口和B口
*@param		无
*@retval	无
*/
void Encoder_Init(void)
{
	// 开启GPIOB和AFIO的时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 设置GPIOB0和GPIOB1的模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 通过AFIO将EXTI0、EXTI1拨到GPIOB
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
	
	// 设置EXTI0和EXTI1的触发方式为下降沿，且为中断触发
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStructure);
	
	// 设置NVIC的优先级组为2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}

/**
*@berif		将旋转编码器的变化值传回主函数
*@param		无
*@retval	temp: 旋转编码器的变化值
*/
int16_t Encoder_GetValue(void)
{
	int16_t temp;
	temp = Cycle_Count;
	Cycle_Count = 0;
	return temp;
}

/**
*@berif		EXIT0的外部中断函数
*@param		无
*@retval	无
*@note 		当GPIOB1的电平为低电平且触发EXTI0外部中断认定为反转
*/
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0))
	{
		if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)){
			Cycle_Count--;
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**
*@berif		EXIT1的外部中断函数
*@param		无
*@retval	无
*@note 		当GPIOB0的电平为低电平且触发EXTI1外部中断认定为反转
*/
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1))
	{
		if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)){
			Cycle_Count++;
		}
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}
