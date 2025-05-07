/*
 * led_chasing.c
 *
 * Created: 4/8/2025 8:18:59 PM
 *  Author: Admin
 */ 
#include "led_chasing.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// PB0 = SH_CP, PB1 = ST_CP, PB2 = DS
#define SH_CP  PD4
#define ST_CP  PD5
#define DS     PD6

// Hàm d?ch 1 byte ra 74HC595
void ShiftOut (uint8_t data)
{
	for (int8_t i = 7; i >= 0; i--) 
	{
		if (data & (1 << i)) PORTD |=  (1 << DS);
		else                 PORTD &= ~(1 << DS);

		PORTD |=  (1 << SH_CP);
		_delay_us(1);
		PORTD &= ~(1 << SH_CP);
		_delay_us(1);
	}
}

// Latch d? li?u lên output
void latch(void) 
{
	PORTD |=  (1 << ST_CP);
	_delay_us(1);
	PORTD &= ~(1 << ST_CP);
	_delay_us(1);
}

// Kh?i t?o các chân
void LED_chase_Init(void) 
{
	DDRD |= (1<<SH_CP)|(1<<ST_CP)|(1<<DS);
	PORTD &= ~((1<<SH_CP)|(1<<ST_CP)|(1<<DS));
}

// Xu?t pattern ra LED
void LED_Write(uint8_t pattern) 
{
	ShiftOut(pattern);       // G?i high byte tr??c
	latch();
}

// Mode 1: LED ch?y t? trái sang ph?i
void Led_chase1(uint8_t *pattern) {
	LED_Write(*pattern);
	_delay_ms(10);

	*pattern <<= 1;
	if (*pattern == 0) *pattern = 0x01;
}

// Mode 2: LED ch?y t? ph?i sang trái
void Led_chase2(uint8_t *pattern) {
	LED_Write(*pattern);
	_delay_ms(100);

	*pattern >>= 1;
	if (*pattern == 0) *pattern = 0x80;
}