/*
 * IncFile1.h
 *
 * Created: 24/04/2025 9:56:46 SA
 *  Author: Lenovo
 */ 

#ifndef I2C_H
#define I2C_H

#include <inttypes.h>
#include <avr/io.h> // ThÍm include n‡y n?u ch?a cÅEtrong I2C.c

// ??nh ngh?a t?c ?? xung clock I2C (SCL) - CÅEth? ?i?u ch?nh
#define I2C_SCL_FREQUENCY 100000UL // 100kHz

// H‡m kh?i t?o giao ti?p I2C
void i2c_init(void);

// H‡m b?t ??u truy?n I2C (g?i START condition)
uint8_t i2c_start(uint8_t address);

// H‡m g?i m?t byte d? li?u qua I2C
uint8_t i2c_write(uint8_t data);

// H‡m d?ng truy?n I2C (g?i STOP condition)
void i2c_stop(void);

#endif // I2C_H