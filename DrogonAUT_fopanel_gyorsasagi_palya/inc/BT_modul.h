/*
 * BT_modul.h
 *
 *  Created on: 2017. okt. 29.
 *      Author: Roland
 */

#ifndef BT_MODUL_H_
#define BT_MODUL_H_

#include "bsp.h"

#define BT_Con_Status_Pin GPIO_PIN_10
#define BT_Con_Status_Port GPIOC
void BT_modul_init();
void BT_init_pins();
uint8_t is_connection_up();
void BT_message_receive_handler();

#endif /* BT_MODUL_H_ */
