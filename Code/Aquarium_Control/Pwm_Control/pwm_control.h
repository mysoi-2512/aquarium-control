#ifndef PWM_CONTROL_H_
#define PWM_CONTROL_H_

#include <stdint.h>

void pwm_init(void);                                    // Khoi tao PWM
void pwm_set_duty(uint8_t duty);                        // Cap nhat gia tri PWM
void pwm_update_based_on_temp(void);              // Dieu chinh PWM theo nhiet do


#endif /* PWM_CONTROL_H_ */