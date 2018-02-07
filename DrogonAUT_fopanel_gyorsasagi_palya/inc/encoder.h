/*
 * encoder.h
 *
 *  Created on: 2018. jan. 6.
 *      Author: Roland
 */

#include "bsp.h"





void Error_Handler();


//CH_A --> PA0
#define ENCODER_CH_A_PIN GPIO_PIN_0
//CH_B --> PA1
#define ENCODER_CH_B_PIN GPIO_PIN_1

//Magnetic field indicator PB9
#define M_FIELD_IND GPIO_PIN_9

#define ENCODER_VALUE_TO_MM 0.0285f
