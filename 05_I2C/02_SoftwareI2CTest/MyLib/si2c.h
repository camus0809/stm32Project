#ifndef _SI2C_H_
#define _SI2C_H_

void My_SoftwareI2C_Init(void);
uint8_t My_SoftwareI2C_SendByte(uint8_t Byte);
uint8_t My_SoftwareI2C_ReceiveByte(uint8_t Ack);
int My_SoftwareI2C_SendBytes(uint8_t Addr, uint8_t *pData, uint16_t size);
int My_SoftwareI2C_ReceiveBytes(uint8_t Addr, uint8_t *pBuffer, uint16_t size);
#endif
