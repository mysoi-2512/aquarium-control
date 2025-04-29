#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "Ds18b20_Sensor/ds18b20_sensor.h"
#include "Pwm_Control/pwm_control.h"
#include "Lcd_Display/lcd_display.h"
#include "I2C/I2C.h"
#include "Uart/uart_utils.h"
#include "interface.h"

#define BAUD 9600
#define UBRR_VALUE F_CPU / 16 / BAUD - 1

SystemState systemState;

int main(void) {
  USART_Init(UBRR_VALUE); // Kh?i t?o UART
  printf("Hello from Arduino!\r\n");
  printf("Chuong trinh doc nhiet do DS18B20, hien thi LCD (I2C) va UART (AVR)\r\n");

  i2c_init();
	ds18b20_init(DS18B20_PIN);
  lcd_init();
  pwm_init();
  
//   while(1){
// 	systemState.temperature = 27.5;
// 	pwm_update_based_on_temp(systemState.temperature, &systemState);
// 	lcd_display_status(&systemState);
//   }
	
  while (1) {
    if (ds18b20_reset()) {
		
        ds18b20_request_temperature();
		
		systemState.temperature = ds18b20_read_temperature();		
		
		pwm_update_based_on_temp(systemState.temperature, &systemState);
	
		lcd_display_status(&systemState);

        printf("Reset Result: 1\r\n"); 

        printf("Nhiet do: %.1f *C\r\n", systemState.temperature);
    } else {
        printf("Reset Result: 0\r\n"); 

        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_send_string("Sensor Error");

        printf("Loi: Khong tim thay cam bien DS18B20\r\n");
	}

    _delay_ms(2000);
}

  return 0;
}