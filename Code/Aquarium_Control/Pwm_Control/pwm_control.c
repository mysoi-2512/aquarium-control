#include "pwm_control.h"
/*#include "../interface.h"*/
#include <avr/io.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define PWM_DDR DDRD
#define PWM_PIN 5
#define LEDRED_PIN 7
#define LEDBLUE_PIN 4
#define COOLER_DDR DDRB
#define COOLER_PORT PORTB
#define COOLER_PIN 0
#define PRESCALE 64
#define PWM_FREQ 1000
#define TOP 249	// TOP = OCR0A

void pwm_init()
{
	PWM_DDR |= (1 << PWM_PIN) | (1 << LEDBLUE_PIN) | (1 << LEDRED_PIN);  // PD5, PD4, PD7 = output
	COOLER_DDR |= (1 << COOLER_PIN);  // PB0 output control relay

	TCCR0A = (1 << WGM01) | (1 << WGM00);
	TCCR0B = (1 << WGM02);
	OCR0A = TOP;
	OCR0B = 0;
}

void pwm_set_duty(uint8_t duty)
{
	if (duty > 100) duty = 100;

	uint8_t ocr_value = ((uint16_t)(OCR0A + 1) * duty) / 100;
	OCR0B = ocr_value;

	if (duty > 0) {
		
		TCCR0A |= (1 << COM0B1);		
		TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00); // prescaler = 64		
		PORTD |= (1 << LEDRED_PIN);   // PD7 ON
		PORTD &= ~(1 << LEDBLUE_PIN);  // PD4 OFF
		} else {
		
		TCCR0A &= ~(1 << COM0B1);		
		TCCR0B = (1 << WGM02);		
		PORTD |= (1 << LEDBLUE_PIN);   // PD4 ON
		PORTD &= ~(1 << LEDRED_PIN);  // PD7 OFF
	}
}

void pwm_update_based_on_temp(float temp, SystemState* state)
{
	uint8_t duty = 0;

	// Mapping temp to duty
	if (temp >= 31.0) duty = 0;
	else if (temp >= 30.0) duty = 0;
	else if (temp >= 25.0) duty = 0;
	else duty = 0;

	// Set duty for PWM
	pwm_set_duty(duty);

	// Update pump status in state
	state->pump_pwm_value = duty;  
	state->pump_status = (duty > 0) ? 1 : 0;  

	if (duty > 0) {
		COOLER_PORT |= (1 << COOLER_PIN); 
		} else {
		COOLER_PORT &= ~(1 << COOLER_PIN);  
	}
}
