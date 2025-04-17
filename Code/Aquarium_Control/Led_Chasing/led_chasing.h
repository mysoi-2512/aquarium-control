/*
 * led_chasing.h
 *
 * Created: 4/8/2025 8:19:10 PM
 *  Author: Admin
 */ 


#ifndef LED_CHASING_H_
#define LED_CHASING_H_
typedef enum {
    LED_OFF,
    LED_ON,
    CHASE_MODE1,
    CHASE_MODE2
} LED_Mode;

void LED_init(void);
void LED_set_mode(LED_Mode mode);
void LED_update(void);

#endif /* LED_CHASING_H_ */