/*
 * gyari_szervo_pwm.h
 *
 *  Created on: 2017. okt. 29.
 *      Author: Roland
 */

#pragma once
#ifndef SZERVO_PWM_H_
#define SZERVO_PWM_H_


#include "bsp.h"

//Min-Max értéke az RC PWM jel pulzusszélességéhez
#define GYARI_SZ_MIN_CMP 3817
#define GYARI_SZ_MAX_CMP 8017

//#define DIGIT_SZ_MIN_CMP 24500
//#define DIGIT_SZ_MIN_CMP 23000
#define DIGIT_SZ_MIN_CMP 25500
//#define DIGIT_SZ_MAX_CMP 38500
//#define DIGIT_SZ_MAX_CMP 40000
#define DIGIT_SZ_MAX_CMP 43000
#define DIGIT_SZ_KOZEP 31500


#define SAJAT_SZ_MIN_CMP 23099
#define SAJAT_SZ_MAX_CMP 40739

extern TIM_HandleTypeDef Tim4Handle;
void Init_GY_Szervo_PWM();
void Init_Digit_Szervo_PWM();
void Init_sajat_Szervo_PWM();
void Init_AF_szervo();
void set_compare_value_gy_szervo(uint16_t);
void set_compare_value_sajat_szervo(uint16_t);
void set_compare_value_digit_szervo(uint16_t);



#endif /* SZERVO_PWM_H_ */
