#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DATA_PIN   PB3
#define CLOCK_PIN  PB5
#define LATCH_PIN  PB2

#define SET(x)   (PORTB |= (1 << (x)))
#define CLR(x)   (PORTB &= ~(1 << (x)))

void led_chasing_init() {
	DDRB |= (1 << DATA_PIN) | (1 << CLOCK_PIN) | (1 << LATCH_PIN);
}

void INT0_init() {
	DDRD &= ~(1 << PD2);    
	PORTD |= (1 << PD2);     

	EICRA |= (1 << ISC01);  
	EICRA &= ~(1 << ISC00);
	EIMSK |= (1 << INT0);    
}

void shiftOut(uint8_t data) {
	for (uint8_t i = 0; i < 8; i++) {
		if (data & (1 << (7 - i)))
		SET(DATA_PIN);
		else
		CLR(DATA_PIN);

		SET(CLOCK_PIN);
		_delay_us(1);
		CLR(CLOCK_PIN);
	}
}

void updateLEDs(uint16_t pattern) {
	uint8_t highByte = (pattern >> 8);   // LED 9–16
	uint8_t lowByte  = (pattern & 0xFF); // LED 1–8

	CLR(LATCH_PIN);
	shiftOut(highByte);  
	shiftOut(lowByte);   
	SET(LATCH_PIN);
	_delay_us(1);
	CLR(LATCH_PIN);
}
