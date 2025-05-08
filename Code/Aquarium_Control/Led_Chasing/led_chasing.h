#ifndef LED_CHASING_H
#define LED_CHASING_H

#include <avr/io.h>

void led_chasing_init(void);
void INT0_init(void);
void updateLEDs(uint16_t pattern);

#endif
