/*
 * IncFile1.h
 *
 * Created: 24/04/2025 10:07:45 SA
 *  Author: Lenovo
 */ 


#ifndef UART_UTILS_H
#define UART_UTILS_H

void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
void USART_PutString(char *str);
int printf(const char *fmt, ...);

#endif // UART_UTILS_H