#ifndef LED_CHASING_H
#define LED_CHASING_H

#include <stdint.h>

void led_chasing_init(void);
void initPCINT(void);
void shiftOut16(uint8_t data);
void latch(void);
void led_off(void);
void led_on_all(void);
void led_right_to_left(void);
void led_left_to_right(void);
void updateLEDs(void);
#endif
