/*
 * IncFile1.h
 *
 * Created: 24/04/2025 9:56:46 SA
 *  Author: Lenovo
 */ 

#ifndef I2C_H
#define I2C_H

#include <inttypes.h>
#include <avr/io.h>

#define I2C_SCL_FREQUENCY 100000UL // 100kHz

void i2c_init(void);

uint8_t i2c_start(uint8_t address);


uint8_t i2c_write(uint8_t data);

void i2c_stop(void);

#endif // I2C_H