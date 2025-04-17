/*
 * servo_control.c
 *
 * Created: 4/8/2025 8:19:26 PM
 *  Author: Admin
 */ 
#define SERVO_PIN PB1

void Servo_init(void) {
    // Fast PWM mode with ICR1
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);
    TCCR1B |= (1 << CS11); // Prescaler 8
    
    ICR1 = 40000; // 20ms period (50Hz)
    OCR1A = 3000; // Center position
    
    DDRB |= (1 << SERVO_PIN);
    TCCR1A |= (1 << COM1A1);
}

void Servo_set_angle(uint8_t angle) {
    OCR1A = 1000 + (uint16_t)(angle * 11.11); // 0°-180° to 1000-2000µs
}