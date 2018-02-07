/*
 * infra_receiver.h
 *
 *  Created on: 2018. jan. 17.
 *      Author: Roland
 */
#include "bsp.h"


#ifndef INFRA_RECEIVER_H_
#define INFRA_RECEIVER_H_

void init_infra_rev_pin();
void init_infra_timer();

TIM_HandleTypeDef Tim3Handle;

#endif /* INFRA_RECEIVER_H_ */
