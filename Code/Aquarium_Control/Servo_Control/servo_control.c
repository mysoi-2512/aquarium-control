#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void servo_init() {
	DDRB |= (1 << PB1);

	TCCR1A |= (1 << COM1A1) | (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);  

	ICR1 = 39999;

	OCR1A = 1000;
}

void servo_rotate_180_then_return() {
	
	OCR1A = 4500;
	_delay_ms(1000);  

	OCR1A = 1000;
	_delay_ms(1000); 
}

void INT1_init() {
	DDRD &= ~(1 << PD3);
	PORTD |= (1 << PD3);

	// C?u h?nh INT1: c?nh xu?ng
	EICRA |= (1 << ISC11);
	EICRA &= ~(1 << ISC10);
	EIMSK |= (1 << INT1);
}
