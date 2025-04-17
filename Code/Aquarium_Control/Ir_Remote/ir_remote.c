/*
 * ir_remote.c
 *
 * Created: 4/8/2025 8:18:15 PM
 *  Author: Admin
 */ 
#include "ir_remote.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile static IR_Command ir_code = IR_NO_CMD;

// Ngắt INT0
ISR(INT0_vect) {
    static uint32_t start_time = 0;
    uint32_t duration = TCNT1 - start_time;
    
    if(duration > 4000) { // Debounce
        uint8_t command = 0;
        for(uint8_t i = 0; i < 32; i++) {
            for(volatile uint16_t d = 0; d < 1600; d++); // Delay 1ms
            if(!(PIND & (1 << PD2))) command |= (1 << (i % 8));
        }
        ir_code = (IR_Command)command;
    }
    start_time = TCNT1;
}

void IR_init(void) {
    EICRA |= (1 << ISC01);  // Falling edge trigger
    EIMSK |= (1 << INT0);   // Enable INT0
    DDRD &= ~(1 << PD2);    // Set input
}

IR_Command IR_get_command(void) {
    IR_Command result = ir_code;
    ir_code = IR_NO_CMD;
    return result;
}