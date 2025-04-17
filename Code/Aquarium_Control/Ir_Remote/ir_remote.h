/*
 * ir_remote.h
 *
 * Created: 4/8/2025 8:18:37 PM
 *  Author: Admin
 */ 


#ifndef IR_REMOTE_H_
#define IR_REMOTE_H_
typedef enum {
    IR_ALL_LED_ON   = 0x45,
    IR_ALL_LED_OFF  = 0x46,
    IR_CHASE_MODE1  = 0x47,
    IR_CHASE_MODE2  = 0x44,
    IR_SERVO_TOGGLE = 0x40,
    IR_NO_CMD       = 0xFF
} IR_Command;

void IR_init(void);
IR_Command IR_get_command(void);




#endif /* IR_REMOTE_H_ */