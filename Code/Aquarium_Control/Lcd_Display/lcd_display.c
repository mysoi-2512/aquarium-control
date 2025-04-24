/*
 * lcd_display.c
 *
 * Created: 4/8/2025 8:20:45 PM
 * Author: Admin
 */

#include "lcd_display.h"
#define F_CPU 16000000UL // ??t t?n s? th?ch anh
#include "../I2C/I2C.h" // ???ng d?n ??n I2C.h
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

// Hàm g?i m?t nibble (4 bits) d? li?u ??n LCD qua I2C
static void lcd_send_nibble_i2c(uint8_t data) {
    uint8_t i2c_data;

    // G?i 4 bits cao + các chân ?i?u khi?n
    i2c_data = (data & 0xF0) | LCD_BL_PIN; // Gi? backlight luôn b?t (có th? ?i?u ch?nh)

    // Enable pulse
    i2c_start(LCD_I2C_ADDR);
    i2c_write(i2c_data | LCD_EN_PIN);
    _delay_us(5); // T?ng th?i gian tr?
    i2c_write(i2c_data & ~LCD_EN_PIN);
    i2c_stop();
    _delay_us(50); // T?ng th?i gian tr?
}

// Hàm g?i m?t byte d? li?u (command ho?c character) ??n LCD qua I2C
void lcd_send_byte_i2c(uint8_t data, uint8_t rs) {
    uint8_t i2c_data_high, i2c_data_low;

    i2c_start(LCD_I2C_ADDR); // B?t ??u giao d?ch I2C

    // G?i 4 bits cao
    i2c_data_high = (data & 0xF0) | LCD_BL_PIN | (rs ? LCD_RS_PIN : 0) | LCD_EN_PIN;
    i2c_write(i2c_data_high);
    //_delay_us(5);
    i2c_data_high &= ~LCD_EN_PIN;
    i2c_write(i2c_data_high);
    //_delay_us(5);

    // G?i 4 bits th?p
    i2c_data_low = ((data << 4) & 0xF0) | LCD_BL_PIN | (rs ? LCD_RS_PIN : 0) | LCD_EN_PIN;
    i2c_write(i2c_data_low);
   // _delay_us(5);
    i2c_data_low &= ~LCD_EN_PIN;
    i2c_write(i2c_data_low);

    i2c_stop(); // K?t thúc giao d?ch I2C
   // _delay_us(100);
}

// Hàm kh?i t?o LCD s? d?ng I2C
void lcd_init(void) {
    i2c_init(); // Kh?i t?o giao ti?p I2C
    _delay_ms(100); // T?ng th?i gian tr?

    // Kh?i t?o LCD ? ch? ?? 4-bit
    lcd_send_nibble_i2c(0x03 << 4);
    _delay_us(500); // T?ng th?i gian tr?
    lcd_send_nibble_i2c(0x03 << 4);
    _delay_us(500); // T?ng th?i gian tr?
    lcd_send_nibble_i2c(0x03 << 4);
    _delay_us(100);  // T?ng th?i gian tr?
    lcd_send_nibble_i2c(0x02 << 4);

    // Cài ??t các ch?c n?ng LCD
    lcd_send_byte_i2c(0x28, 0); // 4-bit, 2 dòng, font 5x8
    lcd_send_byte_i2c(0x0C, 0); // B?t hi?n th?, t?t con tr?, t?t nh?p nháy con tr?
    lcd_clear();
    lcd_send_byte_i2c(0x06, 0); // Ch? ?? d?ch con tr? sang ph?i sau m?i ký t?
}

// Hàm g?i m?t ký t? ??n LCD
void lcd_send_char(char data) {
    lcd_send_byte_i2c(data, 1); // rs = 1 cho d? li?u (ký t?)
}

// Hàm g?i m?t chu?i ??n LCD
void lcd_send_string(const char *str) {
    while (*str) {
		//lcd_send_char('28.0');
        lcd_send_char(*str++);
    }
}

// Hàm ??t con tr? LCD ??n v? trí dòng và c?t
void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address;
    switch (row) {
        case 0: address = 0x00; break;
        case 1: address = 0x40; break;
        default: address = 0x00; break;
    }
    lcd_send_byte_i2c(0x80 | (address + col), 0); // 0x80 là l?nh ??t ??a ch? DDRAM
}

// Hàm xóa màn hình LCD
void lcd_clear(void) {
    lcd_send_byte_i2c(0x01, 0); // 0x01 là l?nh xóa màn hình
    _delay_ms(5); // T?ng th?i gian tr?
}