/*
 * CFile1.c
 *
 * Created: 24/04/2025 9:56:29 SA
 *  Author: Lenovo
 */ 

#include "I2C.h"
#define F_CPU 16000000UL // ??t t?n s? th?ch anh
#include <util/delay.h>
#include <compat/twi.h> // Th? vi?n TWI/I2C

// H�m kh?i t?o giao ti?p I2C
void i2c_init(void) {
	// Thi?t l?p t?c ?? bit (SCL frequency)
	TWBR = ((F_CPU / I2C_SCL_FREQUENCY) - 16) / 2;
	// B?t TWI
	TWCR = (1 << TWEN);
}

// H�m b?t ??u truy?n I2C (g?i START condition)
uint8_t i2c_start(uint8_t address) {
	// G?i START condition
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	// Ch? cho START condition ???c g?i
	while (!(TWCR & (1 << TWINT)));

	// Ki?m tra tr?ng th�i
	if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START) {
		return 1; // L?i: START condition kh�ng th�nh c�ng
	}

	// G?i ??a ch? thi?t b? v� bit ghi (0)
	TWDR = (address << 1) | TW_WRITE;
	// X�a c? TWINT ?? b?t ??u g?i ??a ch?
	TWCR = (1 << TWEN) | (1 << TWINT);
	// Ch? cho ??a ch? ???c g?i v� nh?n ACK
	while (!(TWCR & (1 << TWINT)));

	// Ki?m tra tr?ng th�i ACK
	if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
		return 2; // L?i: Kh�ng nh?n ???c ACK t? slave
	}

	return 0; // Th�nh c�ng
}

// H�m g?i m?t byte d? li?u qua I2C
uint8_t i2c_write(uint8_t data) {
	// ??t d? li?u v�o thanh ghi TWDR
	TWDR = data;
	// X�a c? TWINT ?? b?t ??u g?i d? li?u
	TWCR = (1 << TWEN) | (1 << TWINT);
	// Ch? cho d? li?u ???c g?i v� nh?n ACK
	while (!(TWCR & (1 << TWINT)));

	// Ki?m tra tr?ng th�i ACK
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
		return 1; // L?i: Kh�ng nh?n ???c ACK t? slave
	}

	return 0; // Th�nh c�ng
}

// H�m d?ng truy?n I2C (g?i STOP condition)
void i2c_stop(void) {
	// G?i STOP condition
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
	// Ch? cho STOP condition ???c g?i (th?c t? c� th? kh�ng c?n ch?)
	while (TWCR & (1 << TWSTO));
}
