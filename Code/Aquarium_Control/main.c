/*
 * Aquarium_Control.c
 *
 * Created: 4/8/2025 7:47:52 PM
 * Author : Admin
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "Ir_remote/ir_remote.h"
#include "Led_Chasing/led_chasing.h"
#include "Servo_Control/servo_control.h"

// Mã NEC c?a 5 nút trên remote (ví d? ph? bi?n)
#define IR_ALL_ON    0xFFA25D
#define IR_ALL_OFF   0xFF629D
#define IR_CHASE1    0xFFE21D
#define IR_CHASE2    0xFF22DD
#define IR_SERVO     0xFF02FD

int main(void) {
	IR_Init();
	LED_chase_Init();
	uint16_t pattern = 0x0001;
	bool all_on_flag = false;
	Servo_Init();
	uint8_t chase_mode = 0;

	while (1) {
		if (IR_READY()) {
			uint32_t code = IR_GET_CODE();
			IR_ClearFlag();

			switch (code) {
				case IR_ALL_ON:
				//Toggle all LEDs on/off
				if (!all_on_flag)
				{
					LED_Write(0xFFFF);
					all_on_flag = true;
				}else
				{
					LED_Write(0x0000);
					all_on_flag = false;
				}
				chase_mode = 0;
				break;
				
				case IR_CHASE1:
				//Toggle chase mode 1
				if (chase_mode != 1)
				{
					chase_mode = 1;
					all_on_flag = false;
				}else
				{
					chase_mode = 0;
					LED_Write(0x0000);
				}
				break;
				
				case IR_CHASE2:
				//Toggle chase mode 2
				if (chase_mode != 2)
				{
					chase_mode = 2;
					all_on_flag = false;
				}else
				{
					chase_mode = 0;
					LED_Write(0x0000);
				}
				
				break;
				case IR_SERVO:
				// Quay servo 180° ? gi? 1s ? quay v? 0°
				Servo_SetAngle(180);
				_delay_ms(500);
				Servo_SetAngle(0);
				break;
				default:
				break;
			}
		}

		// N?u ?ang ? ch? ?? chase, ch?y liên t?c
		if (chase_mode == 1) {
			Led_chase1(&pattern);
			} else if (chase_mode == 2) {
			Led_chase2(&pattern);
		}
	}
}
