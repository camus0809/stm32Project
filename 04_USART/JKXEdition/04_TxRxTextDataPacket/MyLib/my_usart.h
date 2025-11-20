#ifndef _MYUSART_H_
#define _MYUSART_H_
#include "stdio.h"

extern char My_USART_RxPacket[];

void My_USART_Init(void);
void My_USART_SendByte(uint8_t Byte);
void My_USART_SendBytes(uint8_t *Pbyte, uint16_t size);
void My_USART_SendString(char *string);
void My_USART_SendNumber(uint32_t number);
void My_USART_Printf(char *format, ...);
uint8_t My_USART_GetRxFlag(void);
#endif
