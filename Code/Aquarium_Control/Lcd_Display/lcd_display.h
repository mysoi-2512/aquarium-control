/*
 * lcd_display.h
 *
 * Created: 4/8/2025 8:20:56 PM
 *  Author: Admin
 */ 


#ifndef LCD_DISPLAY_H_
#define LCD_DISPLAY_H_

#include <stdint.h>

// PCF8574 I2C Address (A0=1, A1=1, A2=1)
#define PCF8574_ADDR 0x27  // 0x27 = 0x20 | (1<<0) | (1<<1) | (1<<2)
                          // Or in binary: 0100 1110

// Rest of the code remains the same
#define LCD_RS      0  // P0 - Register Select
#define LCD_RW      1  // P1 - Read/Write
#define LCD_EN      2  // P2 - Enable
#define LCD_BL      3  // P3 - Backlight
#define LCD_D4      4  // P4 - Data 4
#define LCD_D5      5  // P5 - Data 5
#define LCD_D6      6  // P6 - Data 6
#define LCD_D7      7  // P7 - Data 7

// LCD Commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_write_string(const char* str);
void lcd_write_char(char c);
void lcd_display_temperature(float temp);
void lcd_display_pump_info(uint8_t status, uint8_t pwm);

#endif /* LCD_DISPLAY_H_ */