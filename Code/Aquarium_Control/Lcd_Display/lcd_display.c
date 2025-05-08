#include "lcd_display.h"
#include "../interface.h"
#define F_CPU 16000000UL 
#include "../I2C/I2C.h"
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

static void lcd_send_nibble_i2c(uint8_t data) {
    uint8_t i2c_data;

    i2c_data = (data & 0xF0) | LCD_BL_PIN; 

    // Enable pulse
    i2c_start(LCD_I2C_ADDR);
    i2c_write(i2c_data | LCD_EN_PIN);
    _delay_us(5); 
    i2c_write(i2c_data & ~LCD_EN_PIN);
    i2c_stop();
    _delay_us(50); 
}

void lcd_send_byte_i2c(uint8_t data, uint8_t rs) {
    uint8_t i2c_data_high, i2c_data_low;

    i2c_start(LCD_I2C_ADDR); 

    i2c_data_high = (data & 0xF0) | LCD_BL_PIN | (rs ? LCD_RS_PIN : 0) | LCD_EN_PIN;
    i2c_write(i2c_data_high);
    //_delay_us(5);
    i2c_data_high &= ~LCD_EN_PIN;
    i2c_write(i2c_data_high);
    //_delay_us(5);

    i2c_data_low = ((data << 4) & 0xF0) | LCD_BL_PIN | (rs ? LCD_RS_PIN : 0) | LCD_EN_PIN;
    i2c_write(i2c_data_low);
   // _delay_us(5);
    i2c_data_low &= ~LCD_EN_PIN;
    i2c_write(i2c_data_low);

    i2c_stop(); 
   // _delay_us(100);
}

void lcd_init(void) {
    i2c_init(); 
    _delay_ms(100); 

    lcd_send_nibble_i2c(0x03 << 4);
    _delay_us(500); 
    lcd_send_nibble_i2c(0x03 << 4);
    _delay_us(500); 
    lcd_send_nibble_i2c(0x03 << 4);
    _delay_us(100);  
    lcd_send_nibble_i2c(0x02 << 4);

    lcd_send_byte_i2c(0x28, 0); 
    lcd_send_byte_i2c(0x0C, 0);
    lcd_clear();
    lcd_send_byte_i2c(0x06, 0); 
}

void lcd_send_char(char data) {
    lcd_send_byte_i2c(data, 1); 
}

void lcd_send_string(const char *str) {
    while (*str) {
		//lcd_send_char('28.0');
        lcd_send_char(*str++);
    }
}

void lcd_display_status(const SystemState* state) {
	
	lcd_set_cursor(0, 0);
	char line1[16];
	uint8_t temp_int = (uint8_t)state->temperature;
	uint8_t temp_frac = (uint8_t)((state->temperature - temp_int) * 10);

	snprintf(line1, sizeof(line1), "Temp: %d.%d C", temp_int, temp_frac);	
	lcd_send_string(line1);


	lcd_set_cursor(1, 0);
	char line2[17];
	snprintf(line2, sizeof(line2), "Pump:%s PWM:%3d%%",
	state->pump_status ? "ON " : "OFF", state->pump_pwm_value);
	lcd_send_string(line2);
}


void lcd_set_cursor(uint8_t row, uint8_t col) {
    uint8_t address;
    switch (row) {
        case 0: address = 0x00; break;
        case 1: address = 0x40; break;
        default: address = 0x00; break;
    }
    lcd_send_byte_i2c(0x80 | (address + col), 0); 
}

void lcd_clear(void) {
    lcd_send_byte_i2c(0x01, 0); 
    _delay_ms(5); 
}