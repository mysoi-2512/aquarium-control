#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdint.h>

// ==== Bien dung chung ====

typedef struct {
	float temperature;      // Temp
	uint8_t pump_status;   // Pump Status
	uint8_t pump_pwm_value; // % PWM
} SystemState;

// ==== LCD Display Module ====
// -> Goi boi PWM control va DS18B20 de hien thi thong tin

void lcd_display_status(const SystemState* state);
void lcd_init(void);                                    // Khoi tao LCD (su dung I2C)

// ==== DS18B20 Sensor Module ====
// -> Goi boi PWM control de doc nhiet do

//void ds18b20_init(void);                                // Khoi tao DS18B20
float ds18b20_read_temperature(void);                   // Doc nhiet do tu DS18B20

// ==== PWM Pump Control Module ====
// -> Dieu chinh bom dua tren nhiet do
// -> Goi trong main loop hoac timer

void pwm_init(void);                                    // Khoi tao PWM
void pwm_set_duty(uint8_t duty);                        // Cap nhat gia tri PWM
void pwm_update_based_on_temp(float temp, SystemState* state);              // Dieu chinh PWM theo nhiet do

// ==== Relay Control Module ====
// -> Dieu khien relay cho TEC1
// -> TEC1 bat khi bom dang chay

void relay_init(void);                                  // Khoi tao relay TEC1
void relay_set_tec1(uint8_t on);                        // Bat/tat relay TEC1

// ==== IR Remote Module ====
// -> Goi trong ngat hoac polling

void ir_init(void);                                     // Khoi tao IR
void ir_handle_button(uint8_t button_code);             // Xu ly nut nhan tu remote

// ==== LED Chasing Module ====
// -> Dieu khien LED theo IR

void led_chasing_init(void);                            // Khoi tao LED
void led_chasing_mode1(void);                           // Che do chay LED 1
void led_chasing_mode2(void);                           // Che do chay LED 2
void led_all_on(void);                                  // Bat tat ca LED
void led_all_off(void);                                 // Tat tat ca LED

// ==== Servo Control Module ====
// -> Goi khi nhan phim tu remote

void servo_init(void);                                  // Khoi tao servo
void servo_rotate(void);                                // Quay servo

#endif
