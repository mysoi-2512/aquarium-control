/*
 * ir_remote.c
 *
 * Created: 4/8/2025 8:18:15 PM
 *  Author: Admin
 */ 
#include "ir_remote.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint32_t ir_data = 0;
volatile bool     ir_flag = false;

/*
  NEC protocol decode via INT0 + Timer1:
  - INT0 falling edge bắt bắt đầu khung
  - Đo thời gian low/high, giải mã 32-bit
*/
void IR_Init(void) {
    // PD2 (INT0) input, pull-up
    DDRD &= ~(1<<PD2);
    PORTD |=  (1<<PD2);
    // INT0 on any logical change
    EICRA |=  (1<<ISC00);
    EIMSK |=  (1<<INT0);

    // Timer1 dùng để đo khoảng thời gian (prescaler 8)
    TCCR1B = 0;
    sei();
}

void IR_ClearFlag(void) {
    ir_flag = false;
}

ISR(INT0_vect) {
    static uint8_t  state = 0, bit_cnt = 0;
    static uint32_t data_buf = 0;
    static uint16_t t_start = 0, t_mid = 0;

    switch (state) {
        case 0:
            // Bắt đầu: reset timer, chờ low > 8ms
            TCNT1 = 0;
            TCCR1B = (1<<CS11);
            state = 1;
            break;
        case 1:
            t_start = TCNT1;
            TCCR1B = 0;
            if (t_start > 7000 && t_start < 10000) {
                // đúng khoảng 9ms
                // chờ high 4.5ms
                TCNT1 = 0;
                TCCR1B = (1<<CS11);
                state = 2;
            } else {
                state = 0;
            }
            break;
        case 2:
            t_mid = TCNT1;
            TCCR1B = 0;
            if (t_mid > 3500 && t_mid < 5500) {
                // đúng khoảng 4.5ms => bắt đầu thu 32 bit
                bit_cnt = 0;
                data_buf = 0;
                state = 3;
            } else {
                state = 0;
            }
            break;
        case 3:
            // Thu 32 bit: trên mỗi cạnh falling, đọc độ dài high
            {
                _delay_us(550);
                if (PIND & (1<<PD2)) {
                    data_buf = (data_buf<<1) | 1;
                } else {
                    data_buf = (data_buf<<1);
                }
                bit_cnt++;
                if (bit_cnt >= 32) {
                    ir_data = data_buf;
                    ir_flag = true;
                    state = 0;
                }
            }
            break;
        default:
            state = 0;
    }
}