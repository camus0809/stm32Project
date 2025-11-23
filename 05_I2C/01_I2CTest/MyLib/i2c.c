#include "stm32f10x.h"                  // Device header

/**
*@berif		初始化I2C1
*@param		无
*@retval	无
*/
void My_I2C_Init(void)
{
	//	#1. 对I2C1的引脚PB8和PB9进行初始化
	
	//	开启AFIO时钟对I2C1进行重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
	
	//	初始化PB8和PB9引脚为复用开漏输出模式
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	//	#2. 对I2C1进行初始化
	
	//	开启I2C1的时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	//	施加复位信号
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	//	释放复位信号
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
	I2C_InitTypeDef I2C_InitStructure;
	//	设置波特率为400k
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	//	400k --> Fm 快速模式 故需要设置占空比 默认为2/1
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_Init(I2C1, &I2C_InitStructure);
	
	//	#3. 开启I2C1
	I2C_Cmd(I2C1, ENABLE);
}

/**
*@berif		通过I2C向从机发送size个字节
*@param		I2Cx	I2C接口的名字
*@param		Addr	从机的地址
*@param		pData	待发送的size个字节的数据
*@param		size	待发送的字节数
*@retval	发送状态	0 成功	-1 寻址失败	-2 发送的数据被拒绝
*/
int My_I2C_SendBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t *pData, uint16_t size)
{
	//	#1. 等待I2C总线空闲
	
	//	直到I2C总线空闲
	 while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));  
	
	//	#2. 发送起始位
	
	I2C_GenerateSTART(I2Cx, ENABLE);
	//	直到发送起始位完成
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB));
	
	//	#3. 发送地址
	
	//	提前将AF标志位置为0
	I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
	//	发送7位地址和读写位
	I2C_Send7bitAddress(I2Cx, Addr, I2C_Direction_Transmitter);
	while(1){
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)){
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return -1;	// 寻址失败
		}
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR)) break;
	}
	//	清除ADDR标志位
	I2C_ReadRegister(I2Cx, I2C_Register_SR1);
	I2C_ReadRegister(I2Cx, I2C_Register_SR2);
	
	//	#4. 发送数据
	uint16_t i;
	for(i = 0; i < size; i++){
		while(1){
			if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)){
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return -2;	//	数据拒收
			}
			if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE)) break;
		} 
		I2C_SendData(I2Cx, pData[i]);
		while(1){
			if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)){
				I2C_GenerateSTOP(I2Cx, ENABLE);
				return -2;	//	数据拒收	
			} 
			if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF)) break;
		}
	}
	
	//	#5. 发送停止位
	I2C_GenerateSTOP(I2Cx, ENABLE);
	return 0;
}

/**
*@berif		通过I2C从从机读取size个字节
*@param		I2Cx	I2C接口的名字
*@param		Addr	从机的地址
*@param		pData	接收后存放数据的缓冲区
*@param		size	待读取的字节数
*@retval	读取状态	0 成功	-1 寻址失败
*/
int My_I2C_ReceiveBytes(I2C_TypeDef *I2Cx, uint8_t Addr, uint8_t *pBuffer, uint16_t size)
{
	//	#1. 发送起始位
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB));
	
	//	#2. 寻址阶段
	I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
	I2C_Send7bitAddress(I2Cx, Addr, I2C_Direction_Receiver);
	while(1){
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF)){
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return -1;	// 寻址失败
		}
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR)) break;
	}
	
	//	#3. 接收数据
	//	接收1个字节
	if(size == 1){
		//	清除ADDR标志位
		I2C_ReadRegister(I2Cx, I2C_Register_SR1);
		I2C_ReadRegister(I2Cx, I2C_Register_SR2);
		
		//	发送NCK
		I2C_AcknowledgeConfig(I2Cx, DISABLE);
		//	发送停止位
		I2C_GenerateSTOP(I2Cx, ENABLE);
		//	等待RxNE从0到1 -->即RDR由非空到空
		while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
		//	将RDR中的数据传入pBuffer
		pBuffer[0] = I2C_ReceiveData(I2Cx); 
	//	接收2个字节
	}else if(size == 2){
		//	清除ADDR标志位
		I2C_ReadRegister(I2Cx, I2C_Register_SR1);
		I2C_ReadRegister(I2Cx, I2C_Register_SR2);
		
		//	接收第一个字节
		//	发送ACK
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
		while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
		pBuffer[0] = I2C_ReceiveData(I2Cx);
		
		//	接收第二个字节(即最后一个字节)
		I2C_AcknowledgeConfig(I2Cx, DISABLE);
		I2C_GenerateSTOP(I2Cx, ENABLE);
		while(!I2C_GetFlagStatus(I2Cx,I2C_FLAG_RXNE));
		pBuffer[1] = I2C_ReceiveData(I2Cx);
	//	接收多个字节
	}else{
		//	清除ADDR标志位
		I2C_ReadRegister(I2Cx, I2C_Register_SR1);
		I2C_ReadRegister(I2Cx, I2C_Register_SR2);
		
		//	发送前n-1个字节
		//	发送ACK
		I2C_AcknowledgeConfig(I2Cx, ENABLE);
		for(uint16_t i = 0; i < size - 1; i++){
			// 这里ACK开启使能后，后续主机接收到从机的数据，IIC模块会自动补发ACK，无需手动加ACK
			//	接收前n-1个字节的数据
			while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
			pBuffer[i] = I2C_ReceiveData(I2Cx);
		}
		//	接收最后一个字节的数据
		I2C_AcknowledgeConfig(I2Cx, DISABLE);
		I2C_GenerateSTOP(I2Cx, ENABLE);
		while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
		pBuffer[size - 1] = I2C_ReceiveData(I2Cx);
	}
	return 0;	//	接收成功
}

