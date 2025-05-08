/*
 * CFile1.c
 *
 * Created: 24/04/2025 9:56:29 SA
 *  Author: Lenovo
 */ 

#include "I2C.h"
#define F_CPU 16000000UL 
#include <util/delay.h>
#include <compat/twi.h> 

void i2c_init(void) {
	TWBR = ((F_CPU / I2C_SCL_FREQUENCY) - 16) / 2;
	TWCR = (1 << TWEN);
}

uint8_t i2c_start(uint8_t address) {
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != TW_START && (TWSR & 0xF8) != TW_REP_START) {
		return 1; 
	}

	TWDR = (address << 1) | TW_WRITE;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != TW_MT_SLA_ACK) {
		return 2; 
	}

	return 0; 
}


uint8_t i2c_write(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));

	if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
		return 1; 
	}

	return 0;
}

void i2c_stop(void) {
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
	while (TWCR & (1 << TWSTO));
}
