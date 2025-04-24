#include "uart_utils.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdarg.h>

void USART_Init(unsigned int ubrr) {
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}

void USART_Transmit(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void USART_PutString(char *str) {
	for (int i = 0; str[i] != 0; i++) {
		USART_Transmit(str[i]);
	}
}

int printf(const char *fmt, ...) {
	char buffer[128];
	va_list args;
	va_start(args, fmt);
	int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);
	USART_PutString(buffer);
	return len;
}