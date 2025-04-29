#ifndef PWM_CONTROL_H_
#define PWM_CONTROL_H_

#include <stdint.h>
#include "../interface.h"

void pwm_init(void);
void pwm_set_duty(uint8_t duty);
void pwm_update_based_on_temp(float temp, SystemState* state);


#endif /* PWM_CONTROL_H_ */