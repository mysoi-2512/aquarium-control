/*
 * led_chasing.c
 *
 * Created: 4/8/2025 8:18:59 PM
 *  Author: Admin
 */ 
#include "led_chasing.h"
#include <avr/io.h>

#define LATCH_PIN  PB0
#define DATA_PIN   PB2
#define CLOCK_PIN  PB5

static LED_Mode current_mode = LED_OFF;
static uint16_t led_pattern = 0x0001;
static uint8_t direction = 0;

static void shift_out(uint16_t data) {
    PORTB &= ~(1 << LATCH_PIN);
    
    for(uint8_t i = 0; i < 16; i++) {
        // Set data
        if(data & (1 << (15 - i))) {
            PORTB |= (1 << DATA_PIN);
        } else {
            PORTB &= ~(1 << DATA_PIN);
        }
        
        // Clock pulse
        PORTB |= (1 << CLOCK_PIN);
        PORTB &= ~(1 << CLOCK_PIN);
    }
    
    PORTB |= (1 << LATCH_PIN);
}

void LED_init(void) {
    DDRB |= (1 << LATCH_PIN) | (1 << DATA_PIN) | (1 << CLOCK_PIN);
}

void LED_set_mode(LED_Mode mode) {
    current_mode = mode;
    if(mode == LED_OFF) shift_out(0x0000);
    if(mode == LED_ON) shift_out(0xFFFF);
}

void LED_update(void) {
    switch(current_mode) {
        case CHASE_MODE1:
            led_pattern = (led_pattern << 1) | (led_pattern >> 15);
            shift_out(led_pattern);
            break;
            
        case CHASE_MODE2:
            if(direction) {
                led_pattern <<= 1;
                if(led_pattern & 0x8000) direction = 0;
            } else {
                led_pattern >>= 1;
                if(led_pattern & 0x0001) direction = 1;
            }
            shift_out(led_pattern);
            break;
            
        default:
            break;
    }
}