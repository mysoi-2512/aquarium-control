#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <inttypes.h>
#include "../I2C/I2C.h"
#include "../interface.h"

#define LCD_I2C_ADDR 0x27  

#define LCD_RS_PIN   (1 << 0) // P0
#define LCD_RW_PIN   (1 << 1) // P1
#define LCD_EN_PIN   (1 << 2) // P2
#define LCD_BL_PIN   (1 << 3) // P3 (Backlight)
#define LCD_D4_PIN   (1 << 4) // P4
#define LCD_D5_PIN   (1 << 5) // P5
#define LCD_D6_PIN   (1 << 6) // P6
#define LCD_D7_PIN   (1 << 7) // P7


void lcd_init(void);
void lcd_send_byte_i2c(uint8_t data, uint8_t rs);
void lcd_send_char(char data);
void lcd_send_string(const char *str);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_clear(void);
void lcd_display_status(const SystemState* state);

#endif // LCD_DISPLAY_H