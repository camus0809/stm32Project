#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "i2c.h"
#include "OnBoardLED.h"
uint8_t commands[] = {
	0x00,			//	命令流
	0x8d, 0x14,		//	使能电荷泵
	0xaf,			//	打开屏幕开关
	0xa5,			//	让屏幕全亮
	
};

int main(void)
{
	My_I2C_Init();
	My_OnBoardLED_Init();
	My_I2C_SendBytes(I2C1, 0x78, commands, 5);
	
	uint8_t rcvd;
	My_I2C_ReceiveBytes(I2C1, 0x78, &rcvd, 1);
	
	if(rcvd & (0x01 << 6)){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	}else{
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	}
	while(1)
	{
	}
}
