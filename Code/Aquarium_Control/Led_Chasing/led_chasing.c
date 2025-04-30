 #define F_CPU 16000000UL
#include "led_chasing.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t button_event[4] = {0};

// Define control pins
#define DATA_PIN    PB3
#define CLOCK_PIN   PB5
#define LATCH_PIN   PB2

void led_chasing_init(void) {

    DDRB |= (1 << PB4);

    // Set shift register control pins as output
    DDRB |= (1 << DATA_PIN) | (1 << CLOCK_PIN) | (1 << LATCH_PIN);
    
    // Initialize pins to LOW
    PORTB &= ~((1 << DATA_PIN) | (1 << CLOCK_PIN) | (1 << LATCH_PIN));
}

// Function to initialize PCINT for buttons
void initPCINT(void) {
    // Set up pins for buttons (D14, D15, D16, D17)
    DDRC &= ~(1 << PC0) & ~(1 << PC1) & ~(1 << PC2) & ~(1 << PC3); // Set as input
    PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3); // Enable pull-up resistors

    // Enable PCINT interrupts
    PCICR |= (1 << PCIE2);  // Enable pin change interrupt for port C
    PCMSK2 |= (1 << PCINT8) | (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11);  // Enable interrupt for specific pins
}

// Send one byte to 74HC595
void shiftOut16(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        if (data & (1 << (7 - i))) {
            PORTB |= (1 << DATA_PIN);
        } else {
            PORTB &= ~(1 << DATA_PIN);
        }

        PORTB |= (1 << CLOCK_PIN);
        PORTB &= ~(1 << CLOCK_PIN);
    }
}

// Latch to update output
void latch() {
    PORTB |= (1 << LATCH_PIN);
    _delay_us(1);
    PORTB &= ~(1 << LATCH_PIN);
}


// Turn off all LEDs
void led_off(void) {
    shiftOut16(0x00);
    shiftOut16(0x00);
    latch();
}

// Turn on all LEDs
void led_on_all(void) {
    shiftOut16(0xFF);
    shiftOut16(0xFF);
    latch();
}

// Shift LED from right to left
void led_right_to_left(void) {
    uint16_t led = 0x0001;
    for (uint8_t i = 0; i < 16; i++) {
        shiftOut16((led >> 8) & 0xFF);
        shiftOut16(led & 0xFF);
        latch();
        _delay_ms(100);
        led <<= 1;
    }
}

// Shift LED from left to right
void led_left_to_right(void) {
    uint16_t led = 0x8000;
    for (uint8_t i = 0; i < 16; i++) {
        shiftOut16((led >> 8) & 0xFF);
        shiftOut16(led & 0xFF);
        latch();
        _delay_ms(100);
        led >>= 1;
    }
}
// Function to handle button press and update LED pattern
void updateLEDs(void) {
    if (button_event[0]) { // Button D14 pressed
        led_off();
        button_event[0] = 0;  // Clear the event
    }
    else if (button_event[1]) { // Button D15 pressed
        led_on_all();
        button_event[1] = 0;  // Clear the event
    }
    else if (button_event[2]) { // Button D16 pressed
        led_right_to_left();
        button_event[2] = 0;  // Clear the event
    }
    else if (button_event[3]) { // Button D17 pressed
        led_left_to_right();
        button_event[3] = 0;  // Clear the event
    }
}

ISR(PCINT2_vect) {

    PORTB |= (1 << PB4);  // Bật LED test

    // Check each button and set corresponding event flag
    if (!(PINC & (1 << PC0))) button_event[0] = 1; // D14
    if (!(PINC & (1 << PC1))) button_event[1] = 1; // D15
    if (!(PINC & (1 << PC2))) button_event[2] = 1; // D16
    if (!(PINC & (1 << PC3))) button_event[3] = 1; // D17
}

