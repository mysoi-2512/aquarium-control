/*
 * led_chasing.h
 *
 * Created: 4/8/2025 8:19:10 PM
 *  Author: Admin
 */ 


#ifndef LED_CHASING_H
#define LED_CHASING_H

#include <stdint.h>

// Hàm dịch 1 byte ra 74HC595
void ShiftOut (uint8_t data);

// Latch dữ liệu lên output
void latch(void);
// Khởi tạo các chân
void LED_chase_Init(void);

// Xuất pattern ra LED
void LED_Write(uint8_t pattern);

// Ghi 16-bit pattern ra 2×74HC595
void LED_Write(uint8_t pattern);

// Mode 1: LED chạy từ trái sang phải
void Led_chase1(uint8_t *pattern);
// Mode 2: LED chạy từ phải sang trái
void Led_chase2(uint8_t *pattern);

#endif // LED_CHASING_H