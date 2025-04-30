/*
 * servo_control.h
 *
 * Created: 4/8/2025 8:19:37 PM
 *  Author: Admin
 */ 


#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <stdint.h>

// Khởi tạo PWM cho servo (OC1A, PD5)
void Servo_Init(void);

// Góc 0–180°
void Servo_SetAngle(uint8_t angle);

#endif // SERVO_CONTROL_H