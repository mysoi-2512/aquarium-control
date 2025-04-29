#define F_CPU 16000000UL // ??t t?n s? th?ch anh
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h> // N?u b?n dùng ng?t cho 1-Wire (nâng cao)
#include "../Uart/uart_utils.h" // Gi? s? b?n có file này cho UART

// ??nh ngh?a chân DQ
#define DS18B20_PORT PORTD
#define DS18B20_DDR  DDRD
#define DS18B20_PIN  PIND
#define DS18B20_BIT  PD2 // Thay ??i thành chân b?n ?ã k?t n?i

// --- Các hàm giao ti?p 1-Wire c? b?n (t? tri?n khai) ---
// (N?i dung các hàm OW_Reset, OW_WriteBit, OW_ReadBit, OW_WriteByte, OW_ReadByte nh? ?ã g?i)

// --- Hàm tìm ki?m c?m bi?n ---
// (N?i dung hàm OW_Search và bi?n reset_search nh? ?ã g?i)

// --- Hàm main ---
int main(void) {
	DDRD &= ~(1 << PD2); // ??t chân DQ làm input (ban ??u)
	PORTD |= (1 << PD2);  // Kéo lên m?c cao

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
		// Không làm gì trong loop
	}

	return 0;
}