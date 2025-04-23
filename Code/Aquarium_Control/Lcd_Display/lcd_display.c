/*
 * lcd_display.c
 *
 * Created: 4/8/2025 8:20:45 PM
 *  Author: Admin
 */ 
#define F_CPU 8000000UL
#include "lcd_display.h"
#include "i2c.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

static uint8_t backlight_state = 1;
static uint8_t pcf8574_data = 0;

static void lcd_write_4bit(uint8_t data);
static void lcd_write_cmd(uint8_t cmd);
static void lcd_write_data(uint8_t data);
static void pcf8574_write(void);

void lcd_init(void) {
    i2c_init();
    
    // Set backlight on
    pcf8574_data = (backlight_state << LCD_BL);
    pcf8574_write();
    _delay_ms(50);  // Wait for LCD to power up
    
    // Initialize 4-bit mode
    lcd_write_4bit(0x03);
    _delay_ms(5);
    lcd_write_4bit(0x03);
    _delay_ms(5);
    lcd_write_4bit(0x03);
    _delay_ms(1);
    lcd_write_4bit(0x02);
    
    // Initialize LCD
    lcd_write_cmd(LCD_FUNCTIONSET | 0x08);    // 4-bit mode, 2 lines, 5x8 font
    lcd_write_cmd(LCD_DISPLAYCONTROL | 0x04); // Display on, no cursor
    lcd_write_cmd(LCD_ENTRYMODESET | 0x02);   // Increment cursor
    lcd_clear();
}

void lcd_clear(void) {
    lcd_write_cmd(LCD_CLEARDISPLAY);
    _delay_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t row_offsets[] = {0x00, 0x40};
    lcd_write_cmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void lcd_write_string(const char* str) {
    while(*str) lcd_write_data(*str++);
}

void lcd_write_char(char c) {
    lcd_write_data(c);
}

void lcd_display_temperature(float temp) {
	char buf[16];
	uint8_t temp_int = (uint8_t)temp;
	uint8_t temp_frac = (uint8_t)((temp - temp_int) * 10);

	lcd_set_cursor(0, 0);
	snprintf(buf, sizeof(buf), "Temp: %d.%d C", temp_int, temp_frac);
	lcd_write_string(buf);
}


// void lcd_display_pump_info(uint8_t status, uint8_t pwm) {
//     char buf[16];
//     lcd_set_cursor(1, 0);
//     sprintf(buf, "Pump:%s PWM:%3d%%", status ? "ON " : "OFF", pwm);
//     lcd_write_string(buf);
// }

void lcd_display_pump_info(uint8_t status, uint8_t pwm) {
	char buf[8];
	lcd_set_cursor(1, 0);
	
	lcd_write_string("Pump:");
	lcd_write_string(status ? "ON " : "OFF");
	lcd_write_string(" PWM:");

	snprintf(buf, sizeof(buf), "%3d", pwm);  // an to?n h?n sprintf
	lcd_write_string(buf);
	lcd_write_string("%");
}


static void pcf8574_write(void) {
    i2c_start();
    i2c_write_addr(PCF8574_ADDR);
    i2c_write(pcf8574_data);
    i2c_stop();
}

static void lcd_write_4bit(uint8_t data) {
    // Prepare data bits
    pcf8574_data = (pcf8574_data & 0x0F) | (data << 4);
    if(backlight_state) pcf8574_data |= (1 << LCD_BL);
    
    // Write with EN high
    pcf8574_data |= (1 << LCD_EN);
    pcf8574_write();
    _delay_us(1);
    
    // Write with EN low
    pcf8574_data &= ~(1 << LCD_EN);
    pcf8574_write();
    _delay_us(50);
}

static void lcd_write_cmd(uint8_t cmd) {
    pcf8574_data &= ~(1 << LCD_RS);  // RS = 0 for command
    lcd_write_4bit(cmd >> 4);        // Upper nibble
    lcd_write_4bit(cmd & 0x0F);      // Lower nibble
}

static void lcd_write_data(uint8_t data) {
    pcf8574_data |= (1 << LCD_RS);   // RS = 1 for data
    lcd_write_4bit(data >> 4);       // Upper nibble
    lcd_write_4bit(data & 0x0F);     // Lower nibble
}