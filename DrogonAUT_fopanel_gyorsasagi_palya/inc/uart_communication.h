/*
 * uart_communication.h
 *
 *  Created on: 2017. okt. 27.
 *      Author: Roland
 */

#pragma once
#ifndef UART_COMMUNICATION_H_
#define UART_COMMUNICATION_H_

#include "bsp.h"
#define BT_MESSAGE_SIZE 100

extern uint8_t lastReceivedNumber;
extern uint8_t answer_received;
extern uint8_t last_received_message_size;
//extern uint8_t lastReceivedUartCommand;
extern uint8_t lastReceivedMessage[BT_MESSAGE_SIZE];

extern uint16_t KP_slow;
extern uint16_t KD_slow;
extern uint16_t KP_fast;
extern uint16_t KD_fast;
extern uint8_t stop;
extern int8_t sorszam[32];
extern uint16_t max_motor_value;

extern uint16_t indulas_max;
extern uint16_t gyorsulas_max;
extern uint16_t lassulas_min;
extern uint16_t motor_value;
extern uint16_t szervo_value;
extern uint8_t state;

extern uint8_t meg_jott_a_start_kapu_jele;


//Encoderhez
extern uint32_t ic1, ic2;

#define ROBOT_STATE_SIZE 79
typedef struct RobotState {
	uint16_t sensor_values[32];
	uint8_t r_state;
	uint16_t szervo_value;
	int32_t acel_axes_values[3];
};


HAL_StatusTypeDef BT_UART_Init();
HAL_StatusTypeDef BT_UART_SendString(const char* str);

HAL_StatusTypeDef BT_UART_Send_RobotState(struct RobotState rs);

HAL_StatusTypeDef Radio_UART_Init();

#endif /* UART_COMMUNICATION_H_ */
