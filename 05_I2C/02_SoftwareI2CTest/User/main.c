#include "stm32f10x.h"                  // Device header
#include "si2c.h"

int main(void)
{
	My_SoftwareI2C_Init();
	
	uint8_t commands[] = {
		0x00,		//	命令流
		0x8d, 0x14,	//	使能电荷泵
		0xaf,		//	打开屏幕开关
		0xa5,		//	让屏幕全亮
	};
	My_SoftwareI2C_SendBytes(0x78, commands, 5);
	
	while(1)
	{
	}
}
