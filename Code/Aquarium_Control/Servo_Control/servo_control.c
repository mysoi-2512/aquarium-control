/*
 * servo_control.c
 *
 * Created: 4/8/2025 8:19:26 PM
 *  Author: Admin
 */ 
#include "servo_control.h"
#include <avr/io.h>

void Servo_Init(void) {
	DDRD |= (1<<PD5);
	// Fast PWM, ICR1 = TOP, non-inv (COM1A1)
	TCCR1A = (1<<COM1A1)|(1<<WGM11);
	TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);  // prescaler = 8
	ICR1 = 40000 - 1;  // 50Hz: 16MHz/8/50 = 40000
}

void Servo_SetAngle(uint8_t angle) {
	// 1ms → 2000 counts, 2ms → 4000 counts
	uint16_t ocr = 2000 + ((uint32_t)angle * 2000) / 180;
	OCR1A = ocr;
}