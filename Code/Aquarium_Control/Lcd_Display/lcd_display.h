#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <inttypes.h>
#include "../I2C/I2C.h" // ???ng d?n ??n I2C.h

// Th? l?n l??t các ??a ch? sau và biên d?ch, n?p l?i, ch?y
//#define LCD_I2C_ADDR 0x20
//#define LCD_I2C_ADDR 0x21
//#define LCD_I2C_ADDR 0x22
//#define LCD_I2C_ADDR 0x23
//#define LCD_I2C_ADDR 0x24
//#define LCD_I2C_ADDR 0x25
//#define LCD_I2C_ADDR 0x26
#define LCD_I2C_ADDR 0x27  // B?t ??u v?i ??a ch? này
//#define LCD_I2C_ADDR 0x3F  // Th? thêm ??a ch? ph? bi?n này n?u 0x27 không ???c

// Các ??nh ngh?a cho chân trên PCF8574 t??ng ?ng v?i LCD
#define LCD_RS_PIN   (1 << 0) // P0
#define LCD_RW_PIN   (1 << 1) // P1
#define LCD_EN_PIN   (1 << 2) // P2
#define LCD_BL_PIN   (1 << 3) // P3 (Backlight)
#define LCD_D4_PIN   (1 << 4) // P4
#define LCD_D5_PIN   (1 << 5) // P5
#define LCD_D6_PIN   (1 << 6) // P6
#define LCD_D7_PIN   (1 << 7) // P7

// Hàm kh?i t?o LCD s? d?ng I2C
void lcd_init(void);

// Hàm g?i m?t byte d? li?u (command ho?c character) ??n LCD qua I2C
void lcd_send_byte_i2c(uint8_t data, uint8_t rs);

// Hàm g?i m?t ký t? ??n LCD
void lcd_send_char(char data);

// Hàm g?i m?t chu?i ??n LCD
void lcd_send_string(const char *str);

// Hàm ??t con tr? LCD ??n v? trí dòng và c?t
void lcd_set_cursor(uint8_t row, uint8_t col);

// Hàm xóa màn hình LCD
void lcd_clear(void);

#endif // LCD_DISPLAY_H