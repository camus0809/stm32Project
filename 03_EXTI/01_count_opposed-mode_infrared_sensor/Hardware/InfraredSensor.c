#include "stm32f10x.h"                  // Device header
uint16_t Count_Value;
/**
*@berif		初始化对射式红外传感器	
*@param		无
*@retval	无
*/
void InfraredSensor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// EXTI和NVIC的时钟始终打开，无须手动开启
	  
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_Initstructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 12;
	NVIC_Init(&NVIC_InitStructure);
}

/**
*@berif		获取对射式红外传感器的输入
*@param		无
*@retval	Sensor_Value: 对射式红外传感器的输入数值
*/
uint16_t InfraredSensor_GetValue(void)
{
	return Count_Value;
}

/**
*@berif		EXTI14中断函数
*@param		无
*@retval	无
*/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line14)){
		if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)){
			Count_Value++;
		}
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}
