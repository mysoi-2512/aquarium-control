#define F_CPU 16000000UL
#include "servo_control.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t button_pressed = 0;

void servo_init(void) {
    DDRD &= ~(1 << PD3);    // PD3 input
    PORTD |= (1 << PD3);    // Pull-up

    EICRA |= (1 << ISC11);
    EICRA &= ~(1 << ISC10);
    EIMSK |= (1 << INT1);   

    DDRB |= (1 << PB1);     
}

void servo_pwm_start(void) {
    TCCR1A = (1 << COM1A1) | (1 << WGM11);            // Fast PWM, non-inverting
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler = 8
    ICR1 = 39999; // TOP cho 50Hz (20ms)
}

void servo_pwm_stop(void) {
    TCCR1A = 0;
    TCCR1B = 0;
}

void servo_rotate(void) {
    if (button_pressed) {
        button_pressed = 0;
        EIMSK &= ~(1 << INT1);  

        servo_pwm_start(); 
        
        OCR1A = 4820; _delay_ms(500);  

        servo_pwm_stop();  

        EIMSK |= (1 << INT1);  
    }
}

ISR(INT1_vect) {
    button_pressed = 1;
}
