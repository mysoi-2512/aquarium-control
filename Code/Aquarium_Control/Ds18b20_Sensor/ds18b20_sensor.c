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

// H‡m kh?i t?o ch‚n k?t n?i DS18B20
void ds18b20_init(uint8_t pin) {
	// Thi?t l?p ch‚n l‡m output (ban ??u) vÅEkÈo lÍn m?c cao
	DDRD |= (1 << pin);
	PORTD |= (1 << pin);
}

// G?i xung reset vÅEki?m tra ph?n h?i presence pulse
uint8_t ds18b20_reset(void) {
	uint8_t presence = 0;

	printf("Bat dau reset DS18B20...\r\n");

	// KÈo ch‚n xu?ng m?c th?p trong ˙ë nh?t 480 us
	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	printf("Keo chan xuong...\r\n");
	_delay_us(500);

	// Th? ch‚n vÅEchuy?n sang input ?? nghe presence pulse
	DDRD &= ~(1 << DS18B20_PIN);
	printf("Tha chan...\r\n");
	_delay_us(70);

	// ??c giÅEtr? ch‚n (presence pulse s? kÈo nÅExu?ng th?p trong 60-240 us)
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

// G?i m?t bit trÍn bus 1-Wire
void ds18b20_write_bit(uint8_t bit) {
	// KÈo ch‚n xu?ng m?c th?p
	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	_delay_us(2); // Gi? th?p

	// Ghi bit
	if (bit) {
		DDRD &= ~(1 << DS18B20_PIN); // Th? ch‚n (cho m?c cao)
	}
	_delay_us(60); // Th?i gian slot
	DDRD &= ~(1 << DS18B20_PIN); // ??m b?o th? ch‚n sau khe th?i gian
}

// ??c m?t bit t? bus 1-Wire
uint8_t ds18b20_read_bit(void) {
	uint8_t bit = 0;

	// KÈo ch‚n xu?ng m?c th?p
	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	_delay_us(2);

	// Th? ch‚n ?? b?t ??u khe ??c
	DDRD &= ~(1 << DS18B20_PIN);
	_delay_us(15);

	// ??c giÅEtr? bit
	if (PIND & (1 << DS18B20_PIN)) {
		bit = 1;
	}
	_delay_us(45); // K?t th˙c khe ??c

	return bit;
}

// G?i m?t byte d? li?u trÍn bus 1-Wire
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

// YÍu c?u c?m bi?n b?t ??u ?o nhi?t ??
void ds18b20_request_temperature(void) {
	ds18b20_reset();
	ds18b20_write_byte(0xCC); // Skip ROM (gi? s? ch? cÅEm?t thi?t b?)
	ds18b20_write_byte(0x44); // Convert T
	_delay_ms(750); // Ch? quÅEtrÅEh ?o ho‡n t?t (cÅEth? c?n ?i?u ch?nh)
}

// ??c nhi?t ?? t? c?m bi?n vÅEtr? v? giÅEtr? float (?? C)
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

	// Nhi?t ?? ???c bi?u di?n d??i d?ng s? d?u ph?y ??ng 12-bit (cÅEd?u)
	if (raw_temp & 0x8000) { // Nhi?t ?? ‚m
		raw_temp = (~raw_temp) + 1;
		temperature = -((float)raw_temp / 16.0);
	} else { // Nhi?t ?? d??ng
		temperature = (float)raw_temp / 16.0;
	}

	return temperature;
}