/*
 * ds18b20_sensor.c
 *
 * Created: 4/8/2025 8:21:22 PM
 * Author: Admin
 */
#include "ds18b20_sensor.h"
#define F_CPU 16000000UL 
#include <util/delay.h>
#include "../Uart/uart_utils.h" 

#define DS18B20_PIN PD6

void ds18b20_init() {
	DDRD |= (1 << DS18B20_PIN);
	PORTD |= (1 << DS18B20_PIN);
}

uint8_t ds18b20_reset(void) {
	uint8_t presence = 0;

	printf("Bat dau reset DS18B20...\r\n");

	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	printf("Keo chan xuong...\r\n");
	_delay_us(500);

	DDRD &= ~(1 << DS18B20_PIN);
	printf("Tha chan...\r\n");
	_delay_us(70);

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

void ds18b20_write_bit(uint8_t bit) {

	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	_delay_us(2); 

	if (bit) {
		DDRD &= ~(1 << DS18B20_PIN); 
	}
	_delay_us(60); 
	DDRD &= ~(1 << DS18B20_PIN); 
}

uint8_t ds18b20_read_bit(void) {
	uint8_t bit = 0;

	DDRD |= (1 << DS18B20_PIN);
	PORTD &= ~(1 << DS18B20_PIN);
	_delay_us(2);

	DDRD &= ~(1 << DS18B20_PIN);
	_delay_us(15);

	if (PIND & (1 << DS18B20_PIN)) {
		bit = 1;
	}
	_delay_us(45); 

	return bit;
}

void ds18b20_write_byte(uint8_t data) {
	for (uint8_t i = 0; i < 8; i++) {
		ds18b20_write_bit(data & (1 << i));
	}
}

uint8_t ds18b20_read_byte(void) {
	uint8_t data = 0;
	for (uint8_t i = 0; i < 8; i++) {
		if (ds18b20_read_bit()) {
			data |= (1 << i);
		}
	}
	return data;
}

void ds18b20_request_temperature(void) {
	ds18b20_reset();
	ds18b20_write_byte(0xCC); 
	ds18b20_write_byte(0x44); 
	_delay_ms(750); 
}

float ds18b20_read_temperature(void) {
	uint8_t temp_low, temp_high;
	int16_t raw_temp;
	float temperature;

	ds18b20_reset();
	ds18b20_write_byte(0xCC); 
	ds18b20_write_byte(0xBE); 

	temp_low = ds18b20_read_byte();
	temp_high = ds18b20_read_byte();

	raw_temp = (temp_high << 8) | temp_low;

	if (raw_temp & 0x8000) { 
		raw_temp = (~raw_temp) + 1;
		temperature = -((float)raw_temp / 16.0);
	} else { 
		temperature = (float)raw_temp / 16.0;
	}

	return temperature;
}