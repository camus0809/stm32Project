#include "stm32f10x.h"                  // Device header
#define My_SCL	GPIO_Pin_0
#define My_SDA	GPIO_Pin_1
/**
*@berif		初始化PA0和PA1分别作为SCL和SDA
*@param		无
*@retval	无
*/
void My_SoftwareI2C_Init(void)
{
	//	#1. 初始化PA0 PA1引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
}
/**
*@berif		向SCL写值
*@param		level	待写入的0或者1
*@retval	无
*/
void SCL_Write(uint8_t level)
{
	if(!level){
		GPIO_WriteBit(GPIOA, My_SCL, Bit_RESET);
	}else{
		GPIO_WriteBit(GPIOA, My_SCL, Bit_SET);
	}
}
/**
*@berif		向SDA写值
*@param		level	待写入的0或者1
*@retval	无
*/
void SDA_Write(uint8_t level)
{
	if(!level){
		GPIO_WriteBit(GPIOA, My_SDA, Bit_RESET);
	}else{
		GPIO_WriteBit(GPIOA, My_SDA, Bit_SET);
	}
}
/**
*@berif		读取SDA上的数据
*@param		无
*@retval	SDA上的数据
*/
uint8_t SDA_Read(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, My_SDA)){
		return 1;
	}else{
		return 0;
	}
}

/**
*@berif		微秒级延时
*@param		xus	需要延时xus微秒
*@retval	无
*/
void Delay_us(uint32_t xus)
{
    uint32_t n = xus * 8;
    for(uint32_t i = 0; i < n; i++);
}

/**
*@berif		发送起始位
*@param		无
*@retval	无
*/
void Send_StartBit(void)
{
	//	#1. 向SDA写0,下降沿
	SDA_Write(0);
	Delay_us(1);
}

/**
*@berif		发送停止位
*@param		无
*@retval	无
*/
void Send_StopBit(void)
{
	//	#1. 保证SDA为低电平 --> 上升沿	保证SCL为低电平 --> 才能改变SDA的电平
	SCL_Write(0);
	SDA_Write(0);
	Delay_us(1);
	//	#2. 释放SCL
	SCL_Write(1);
	Delay_us(1);
	//	#3. 释放SDA发送上升沿 --> 停止位
	SDA_Write(1);
	Delay_us(1);
}

/**
*@berif		发送一个字节
*@param		Byte	待发送的一个字节的数据
*@retval	接收方是否应答 1为应答 0为拒收
*/
uint8_t My_SoftwareI2C_SendByte(uint8_t Byte)
{
	//	#1. 发送一个字节的数据
	for(int8_t i = 7; i >= 0; i--){
		SCL_Write(0);
		if(Byte&(0x01 << i)){
			SDA_Write(1);
		}else{
			SDA_Write(0);
		}
		Delay_us(1);
		
		SCL_Write(1);
		Delay_us(1);
	}
	
	//	#2. 读取ACK或NAK
	SCL_Write(0);	// 为了能更改SDA的电平
	SDA_Write(1);	//	释放SDA
	Delay_us(1);
	SCL_Write(1);	//	拉高SCL
	Delay_us(1);	
	
	return SDA_Read();	// 读取SDA作为ACK或者NAK
}

/**
*@berif		接收一个字节
*@param		Ack		发送的确认位
*@retval	Byte	接收的一个字节
*/
uint8_t My_SoftwareI2C_ReceiveByte(uint8_t Ack)
{
	uint8_t Byte = 0;
	//	#1. 接收一个字节的数据	有符号整型 --> 为了防止死循环
	for(int8_t i = 7; i >= 0; i--){
		SCL_Write(0);
		SDA_Write(1);
		Delay_us(1);
		SCL_Write(1);
		Delay_us(1);
		
		if(SDA_Read()){
			Byte |= 0x01 << i;
		}
	}
	
	//	#2. 发送ACK
	SCL_Write(0);
	SDA_Write(!Ack);	//	与I2C规定的ACK相反
	Delay_us(1);
	
	SCL_Write(1);
	Delay_us(1);
	
	//	#3. 返回接收到的数值
	return Byte;
}

/**
*@berif		发送多个字节
*@param		Addr
			pData
			size
*@retval	是否成功
*/
int My_SoftwareI2C_SendBytes(uint8_t Addr, uint8_t *pData, uint16_t size)
{
	Send_StartBit();
	if(My_SoftwareI2C_SendByte(Addr & 0xFE)){
		Send_StopBit();
		return -1;	// 寻址失败
	}
	
	for(uint16_t i = 0; i < size; i++){
		if(My_SoftwareI2C_SendByte(pData[i])){
			Send_StopBit();
			return -2; //	数据拒收
		}
	}
	Send_StopBit();
	return 0;
}

/**
*@berif		接收多个字节
*@param		Addr
			pData
			size
*@retval	是否成功
*/
int My_SoftwareI2C_ReceiveBytes(uint8_t Addr, uint8_t *pBuffer, uint16_t size)
{
	Send_StartBit();
	if(My_SoftwareI2C_SendByte(Addr | 0x01)){
		Send_StopBit();
		return -1;	
	}
	for(int16_t i = 0; i < size - 1; i++){
		pBuffer[i] = My_SoftwareI2C_ReceiveByte(1);
	}
	pBuffer[size - 1] = My_SoftwareI2C_ReceiveByte(0);
	Send_StopBit();
	return 0;
}
