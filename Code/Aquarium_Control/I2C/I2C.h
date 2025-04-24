/*
 * IncFile1.h
 *
 * Created: 24/04/2025 9:56:46 SA
 *  Author: Lenovo
 */ 

#ifndef I2C_H
#define I2C_H

#include <inttypes.h>
#include <avr/io.h> // Thêm include này n?u ch?a có trong I2C.c

// ??nh ngh?a t?c ?? xung clock I2C (SCL) - Có th? ?i?u ch?nh
#define I2C_SCL_FREQUENCY 100000UL // 100kHz

// Hàm kh?i t?o giao ti?p I2C
void i2c_init(void);

// Hàm b?t ??u truy?n I2C (g?i START condition)
uint8_t i2c_start(uint8_t address);

// Hàm g?i m?t byte d? li?u qua I2C
uint8_t i2c_write(uint8_t data);

// Hàm d?ng truy?n I2C (g?i STOP condition)
void i2c_stop(void);

#endif // I2C_H