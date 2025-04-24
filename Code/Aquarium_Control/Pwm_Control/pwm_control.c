#include "pwm_control.h"
#include "../interface.h"

#include <avr/io.h>

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

// void pwm_init()
// {
// 	PWM_PORT |= (1<<PWM_PIN);	//PD5 (OC0B) = output
// 	TCCR0A = (1<<COM0B1) | (1<<WGM01) | (1<<WGM00);	// Timer0 FPWM7 Non-inverting OC0B
// 	TCCR0B = (1<<WGM02) | (1<<CS01) | (1<<CS00);	// N=64
// 	OCR0A = TOP;
// 	OCR0B = 0;
// }

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

	g_pump_pwm_value = ((uint16_t)(duty * 255)) / 100;
}




// void pwm_set_duty(uint8_t duty)
// {
// 	if (duty > 100) duty = 100;
//
// 	// Calculate OCR0B base on TOP and duty
// 	uint8_t ocr_value = ((uint16_t)(OCR0A +1) * duty) / 100;
// 	OCR0B = ocr_value;
//
// 	// Return value to display on LCD
// 	g_pump_pwm_value = ((uint16_t)(duty * 255)) / 100;
// }

void pwm_update_based_on_temp(void)
{
	// Get temp from DS18B20
	//ds18b20_data_t sensor_data = ds18b20_read_temperature();
	//float temp = sensor_data.temperature;
	
	float temp = g_current_temperature;
	uint8_t duty = 0;
	
	// Mapping temp - duty
	if (temp >= 29.0) duty = 100;
	else if (temp >= 28.0) duty = 75;
	else if (temp >= 27.5) duty = 30;
	else duty = 0;
	
	// Return new duty for timer0
	pwm_set_duty(duty);
	
	// Return pump status to display on LCD
	g_pump_status = (duty > 0) ? 1 : 0;
	
	if (g_pump_status) {
		COOLER_PORT |= (1 << COOLER_PIN);  // B?t relay
	} else {
		COOLER_PORT &= ~(1 << COOLER_PIN);  // T?t relay
	}
}

