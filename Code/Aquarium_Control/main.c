#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "Ds18b20_Sensor/ds18b20_sensor.h"
#include "Pwm_Control/pwm_control.h"
#include "Lcd_Display/lcd_display.h"
#include "I2C/I2C.h"
#include "Uart/uart_utils.h"
#include "interface.h"
#include "Servo_Control/servo_control.h"
#include "Led_Chasing/led_chasing.h"

#define BAUD 9600
#define UBRR_VALUE F_CPU / 16 / BAUD - 1

volatile uint8_t trigger = 0;
volatile uint8_t mode = 0;  
SystemState systemState;

ISR(INT0_vect) {
	_delay_ms(50);  
	if (!(PIND & (1 << PD2))) {
		mode++;
		if (mode > 3) mode = 0;
	}
}

ISR(INT1_vect) {
	_delay_ms(50);
	if (!(PIND & (1 << PD3))) {
		trigger = 1;
	}
}

int main(void) {
  USART_Init(UBRR_VALUE); 
  printf("Hello from Arduino!\r\n");
  printf("Chuong trinh doc nhiet do DS18B20, hien thi LCD (I2C) va UART (AVR)\r\n");

	i2c_init();
	ds18b20_init();
	lcd_init();
	pwm_init();
	servo_init();  
	led_chasing_init();
	INT0_init();
	INT1_init();
  
  sei();    
  while (1) {
    if (ds18b20_reset()) {	
		
		// Get temp	
        ds18b20_request_temperature();		
		systemState.temperature = ds18b20_read_temperature();
		
		// Update PWM duty		
		pwm_update_based_on_temp(systemState.temperature, &systemState);
		
		// Show pump state	
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

    // Check mode and handle led chasing
	switch (mode) {
		case 0:
		updateLEDs(0x0000);  // T?t t?t c? LED
		_delay_ms(100);
		break;

		case 1:
		updateLEDs(0xFFFF);  
		_delay_ms(100);
		break;

		case 2: {  
			uint16_t pattern = 0;
			for (uint8_t i = 0; i < 16 && mode == 2; i++) {
				pattern |= (1 << i);
				updateLEDs(pattern);
				_delay_ms(150);
			}
			break;
		}

		case 3: { 
			uint16_t pattern = 0;
			for (int8_t i = 15; i >= 0 && mode == 3; i--) {
				pattern |= (1 << i);
				updateLEDs(pattern);
				_delay_ms(150);
			}
			break;
		}
	}
	
	// handle servo
	if (trigger) {
		servo_rotate_180_then_return();
		trigger = 0;
	}

    _delay_ms(2000);
  }
  
  return 0;
}
