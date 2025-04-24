/*
 * Aquarium_Control.c
 *
 * Created: 4/8/2025 7:47:52 PM
 * Author : Admin
 */

#define F_CPU 16000000UL // ??t t?n s? th?ch anh c?a Arduino UNO
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "Ds18b20_Sensor/ds18b20_sensor.h"
#include "Lcd_Display/lcd_display.h"
#include "I2C/I2C.h"
#include "Uart/uart_utils.h" // Include file Uart/uart_utils.h

#define BAUD 9600
#define UBRR_VALUE F_CPU / 16 / BAUD - 1

int main(void) {
  USART_Init(UBRR_VALUE); // Kh?i t?o UART
  printf("Hello from Arduino!\r\n");
  printf("Chuong trinh doc nhiet do DS18B20, hien thi LCD (I2C) va UART (AVR)\r\n");

  i2c_init();
  ds18b20_init(DS18B20_PIN);
  lcd_init();

  //char temp_str_lcd[20];

  while (1) {
    if (ds18b20_reset()) {
        ds18b20_request_temperature();
        float temperatureC = ds18b20_read_temperature();
        //float temperatureC = 28.5;
		char test_str_lcd[20];

        printf("Reset Result: 1\r\n"); // In k?t qu? reset thành công

        // Hi?n th? lên LCD
        lcd_clear(); // Thêm l?nh xóa màn hình tr??c khi in
        sprintf(test_str_lcd, "Temp: %d*C", (int)temperatureC);
        lcd_set_cursor(0, 0);
        //lcd_send_string(temp_str_lcd);
		lcd_send_string(test_str_lcd);

        // G?i qua UART b?ng printf
        printf("Nhiet do: %.1f *C\r\n", temperatureC);
    } else {
        printf("Reset Result: 0\r\n"); // In k?t qu? reset th?t b?i

        // Hi?n th? l?i trên LCD
        lcd_clear(); // Thêm l?nh xóa màn hình tr??c khi in l?i
        lcd_set_cursor(0, 0);
        lcd_send_string("Sensor Error");

        // G?i thông báo l?i qua UART b?ng printf
        printf("Loi: Khong tim thay cam bien DS18B20\r\n");
    }

    _delay_ms(1000);
}

  return 0;
}