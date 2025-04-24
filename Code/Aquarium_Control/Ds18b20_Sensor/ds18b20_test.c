#define F_CPU 16000000UL // ??t t?n s? th?ch anh
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h> // N?u b?n d�ng ng?t cho 1-Wire (n�ng cao)
#include "../Uart/uart_utils.h" // Gi? s? b?n c� file n�y cho UART

// ??nh ngh?a ch�n DQ
#define DS18B20_PORT PORTD
#define DS18B20_DDR  DDRD
#define DS18B20_PIN  PIND
#define DS18B20_BIT  PD2 // Thay ??i th�nh ch�n b?n ?� k?t n?i

// --- C�c h�m giao ti?p 1-Wire c? b?n (t? tri?n khai) ---
// (N?i dung c�c h�m OW_Reset, OW_WriteBit, OW_ReadBit, OW_WriteByte, OW_ReadByte nh? ?� g?i)

// --- H�m t�m ki?m c?m bi?n ---
// (N?i dung h�m OW_Search v� bi?n reset_search nh? ?� g?i)

// --- H�m main ---
int main(void) {
	DDRD &= ~(1 << PD2); // ??t ch�n DQ l�m input (ban ??u)
	PORTD |= (1 << PD2);  // K�o l�n m?c cao

	USART_Init(9600); // Kh?i t?o UART

	printf("Tim kiem cam bien DS18B20...\n");

	uint8_t address[8];
	if (OW_Search(address)) {
		printf("Tim thay cam bien voi dia chi ROM: ");
		for (int i = 0; i < 8; i++) {
			printf("%02X ", address[i]);
		}
		printf("\n");
		} else {
		printf("Khong tim thay cam bien DS18B20!\n");
		printf("Hay kiem tra ket noi.\n");
	}

	while (1) {
		// Kh�ng l�m g� trong loop
	}

	return 0;
}