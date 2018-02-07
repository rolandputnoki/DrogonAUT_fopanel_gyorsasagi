/*
 * infra_receiver.c

 *
 *  Created on: 2018. jan. 17.
 *      Author: Roland
 */
#include "infra_receiver.h"

TIM_IC_InitTypeDef TIM_ICInitStructure;
extern TIM_HandleTypeDef Tim3Handle;


void init_infra_rev_pin()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	__GPIOB_CLK_ENABLE();
	// GPIO alternate function lábak
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure.Pin =  GPIO_PIN_4;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void init_infra_timer()
{
	//84 MHz
	__TIM3_CLK_ENABLE();

	Tim3Handle.Instance = TIM3;
	Tim3Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim3Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim3Handle.Init.Prescaler = 4199;
	Tim3Handle.Init.Period = 0xFFFF;
	Tim3Handle.State = HAL_TIM_STATE_RESET;


	HAL_TIM_Base_Init(&Tim3Handle);
	HAL_TIM_IC_Init(&Tim3Handle);

	TIM_ICInitStructure.ICFilter = 15;
	TIM_ICInitStructure.ICSelection	= TIM_ICSELECTION_DIRECTTI;
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;
	TIM_ICInitStructure.ICPrescaler = TIM_ETRPRESCALER_DIV1;
	HAL_TIM_IC_ConfigChannel(&Tim3Handle, &TIM_ICInitStructure, TIM_CHANNEL_1);

	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	Tim3Handle.Instance->CCR1 = 0;
	HAL_TIM_IC_Start_IT(&Tim3Handle, TIM_CHANNEL_1);
}

void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&Tim3Handle);
}

