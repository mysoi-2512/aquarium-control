/*
 * ds18b20_sensor.c
 *
 * Created: 4/8/2025 8:21:22 PM
 * Author: Admin
 */
#include "ds18b20_sensor.h"
#define F_CPU 16000000UL // ??t t?n s? th?ch anh
#include <util/delay.h>
#include "../Uart/uart_utils.h" // ??m b?o include file UART

// Hàm kh?i t?o chân k?t n?i DS18B20
void ds18b20_init(uint8_t pin) {
	// Thi?t l?p chân làm output (ban ??u) và kéo lên m?c cao
	DDRD |= (1 << pin);
	PORTD |= (1 << pin);
}

// G?i xung reset và ki?m tra ph?n h?i presence pulse
uint8_t ds18b20_reset(void) {
	uint8_t presence = 0;

	printf("Bat dau reset DS18B20...\r\n");

	// Kéo chân xu?ng m?c th?p trong ít nh?t 480 us
	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	printf("Keo chan xuong...\r\n");
	_delay_us(500);

	// Th? chân và chuy?n sang input ?? nghe presence pulse
	DDRD &= ~(1 << DS18B20_PIN);
	printf("Tha chan...\r\n");
	_delay_us(70);

	// ??c giá tr? chân (presence pulse s? kéo nó xu?ng th?p trong 60-240 us)
	if (!(PIND & (1 << DS18B20_PIN))) {
		printf("Phat hien presence pulse!\r\n");
		presence = 1;
	} else {
		printf("KHONG phat hien presence pulse!\r\n");
	}
	_delay_us(500);

	printf("Ket thuc reset, presence = %d\r\n", presence);
	return presence;
}

// G?i m?t bit trên bus 1-Wire
void ds18b20_write_bit(uint8_t bit) {
	// Kéo chân xu?ng m?c th?p
	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	_delay_us(2); // Gi? th?p

	// Ghi bit
	if (bit) {
		DDRD &= ~(1 << DS18B20_PIN); // Th? chân (cho m?c cao)
	}
	_delay_us(60); // Th?i gian slot
	DDRD &= ~(1 << DS18B20_PIN); // ??m b?o th? chân sau khe th?i gian
}

// ??c m?t bit t? bus 1-Wire
uint8_t ds18b20_read_bit(void) {
	uint8_t bit = 0;

	// Kéo chân xu?ng m?c th?p
	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	_delay_us(2);

	// Th? chân ?? b?t ??u khe ??c
	DDRD &= ~(1 << DS18B20_PIN);
	_delay_us(15);

	// ??c giá tr? bit
	if (PIND & (1 << DS18B20_PIN)) {
		bit = 1;
	}
	_delay_us(45); // K?t thúc khe ??c

	return bit;
}

// G?i m?t byte d? li?u trên bus 1-Wire
void ds18b20_write_byte(uint8_t data) {
	for (uint8_t i = 0; i < 8; i++) {
		ds18b20_write_bit(data & (1 << i));
	}
}

// ??c m?t byte d? li?u t? bus 1-Wire
uint8_t ds18b20_read_byte(void) {
	uint8_t data = 0;
	for (uint8_t i = 0; i < 8; i++) {
		if (ds18b20_read_bit()) {
			data |= (1 << i);
		}
	}
	return data;
}

// Yêu c?u c?m bi?n b?t ??u ?o nhi?t ??
void ds18b20_request_temperature(void) {
	ds18b20_reset();
	ds18b20_write_byte(0xCC); // Skip ROM (gi? s? ch? có m?t thi?t b?)
	ds18b20_write_byte(0x44); // Convert T
	_delay_ms(750); // Ch? quá trình ?o hoàn t?t (có th? c?n ?i?u ch?nh)
}

// ??c nhi?t ?? t? c?m bi?n và tr? v? giá tr? float (?? C)
float ds18b20_read_temperature(void) {
	uint8_t temp_low, temp_high;
	int16_t raw_temp;
	float temperature;

	ds18b20_reset();
	ds18b20_write_byte(0xCC); // Skip ROM
	ds18b20_write_byte(0xBE); // Read Scratchpad

	temp_low = ds18b20_read_byte();
	temp_high = ds18b20_read_byte();

	raw_temp = (temp_high << 8) | temp_low;

	// Nhi?t ?? ???c bi?u di?n d??i d?ng s? d?u ph?y ??ng 12-bit (có d?u)
	if (raw_temp & 0x8000) { // Nhi?t ?? âm
		raw_temp = (~raw_temp) + 1;
		temperature = -((float)raw_temp / 16.0);
	} else { // Nhi?t ?? d??ng
		temperature = (float)raw_temp / 16.0;
	}

	return temperature;
}