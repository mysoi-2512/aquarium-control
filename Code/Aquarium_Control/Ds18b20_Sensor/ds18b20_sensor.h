/*
 * ds18b20_sensor.h
 *
 * Created: 4/8/2025 8:21:43 PM
 *  Author: Admin
 */ 


#ifndef DS18B20_SENSOR_H
#define DS18B20_SENSOR_H

#include <avr/io.h>

// ??nh ngh?a chân k?t n?i v?i bus 1-Wire
#define DS18B20_PIN PD2 // *** QUAN TR?NG: XÁC NH?N CHÂN K?T N?I TH?C T? TRÊN ARDUINO (ví d?: D2 t??ng ?ng PD2) ***

// Khai báo các hàm
void ds18b20_init(uint8_t pin);
uint8_t ds18b20_reset(void);
void ds18b20_write_bit(uint8_t bit);
uint8_t ds18b20_read_bit(void);
void ds18b20_write_byte(uint8_t data);
uint8_t ds18b20_read_byte(void);
void ds18b20_request_temperature(void);
float ds18b20_read_temperature(void);

#endif // DS18B20_SENSOR_H