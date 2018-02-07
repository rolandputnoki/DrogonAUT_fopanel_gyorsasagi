/*
 * gyari_motor_pwm.h
 *
 *  Created on: 2017. okt. 27.
 *      Author: Roland
 */
#pragma once
#ifndef MOTOR_PWM_H_
#define MOTOR_PWM_H_

#include "bsp.h"

#define SAJAT_MOTOR_COUNTER_MAX 4199

/*
#define GYARI_MOTOR_COUNTER_MAX	7200
#define GYARI_MOTOR_COUNTER_MIN 4800
*/


//#define GYARI_MOTOR_COUNTER_MAX	6600
//#define GYARI_MOTOR_COUNTER_MAX	6700
#define GYARI_MOTOR_COUNTER_MAX	7200
//#define GYARI_MOTOR_COUNTER_MIN 5600
#define GYARI_MOTOR_COUNTER_MIN 4800
#define GYARI_MOTOR_COUNTER_KOZEP 6200

extern TIM_HandleTypeDef Tim1Handle, Tim4Handle, Tim7Handle;
void Init_gyari_motor_PWM();
void InitAF_gyari_motor();
void Init_sajat_motor_PWM();
void InitAF_sajat_motor();
void set_compare_value_sajat_motor(uint16_t);
void enable_fet_drivers(uint8_t);




#endif /* MOTOR_PWM_H_ */
