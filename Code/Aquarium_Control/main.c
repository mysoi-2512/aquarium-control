/*
 * Aquarium_Control.c
 *
 * Created: 4/8/2025 7:47:52 PM
 * Author : Admin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

volatile uint8_t mode =0; // 0: OFF, 1: All ON, 2: Chase1, 3: Chase2
volatile bool servo_trigger = false;
#include "Led_Chasing/led_chasing.h"
#include "Servo_Control/servo_control.h"

void External_Interrupt_Init(void) {
	// INT0 - chuy?n mode (PD2)
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);    // pull-up
	// INT1 - ?i?u khi?n servo (PD3)
	DDRD &= ~(1 << PD3);
	PORTD |= (1 << PD3);

	// Falling edge
	EICRA |= (1 << ISC01);
	EICRA |= (1 << ISC11);
	EIMSK |= (1 << INT0) | (1 << INT1);
	sei();
}
ISR(INT0_vect) {
	 // Debounce
	 _delay_ms(50);
	 if (PIND & (1 << PD2)) return; // b? qua nhi?u n?u ch?a th?c s? nh?n

	 mode = (mode + 1) % 4;
}

ISR(INT1_vect) {
	 // Debounce
	 _delay_ms(50);
	 if (PIND & (1 << PD3)) return;

	 servo_trigger = true;
}
int main(void) {
	External_Interrupt_Init();
	LED_chase_Init();
	uint16_t pattern = 0x0001;
	Servo_Init();
	uint8_t chase_mode = 0;

	while (1) {
		switch (mode)
		{
			case 0:
			LED_Write(0x0000);
			break;
			
			case 1:
			LED_Write(0xFFFF);
			break;
			
			case 2:
			Led_chase1(&pattern);
			LED_Write(pattern);
			_delay_ms(100);
			break;
			
			case 3:
			Led_chase2(&pattern);
			LED_Write(pattern);
			_delay_ms(100);
			break;
		}
		if (servo_trigger)
		{
			Servo_SetAngle(180);
			_delay_ms(1000);
			Servo_SetAngle(0);
			servo_trigger = false;
		}
	}
return 0;
}

