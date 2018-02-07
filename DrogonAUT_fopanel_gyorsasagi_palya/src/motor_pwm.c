/*
 * gyari_motor_pwm.c
 *
 *  Created on: 2017. okt. 27.
 *      Author: Roland
 */
#include <motor_pwm.h>

//#include "szervo_pwm.h"
#include "motor_pwm.h"
TIM_HandleTypeDef Tim1Handle, Tim4Handle;


// Alternate function GPIO lábak inicializálása
void InitAF_gyari_motor()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	__GPIOA_CLK_ENABLE();
	// GPIO alternate function lábak
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;

	GPIO_InitStructure.Pin =  GPIO_PIN_9;
	GPIO_InitStructure.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);


}

/*
void InitAF_sajat_motor()
{
	//PWM lábak a fet driverek irányításához
	GPIO_InitTypeDef  GPIO_InitStructure;
	__GPIOA_CLK_ENABLE();
	// GPIO alternate function lábak
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;

	GPIO_InitStructure.Pin =  GPIO_PIN_9 | GPIO_PIN_10;
	GPIO_InitStructure.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);


	//Engedélyező láb a fet driverekhez

	__GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStructure2;
	GPIO_InitStructure2.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure2.Pull = GPIO_NOPULL;
	GPIO_InitStructure2.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure2.Pin =  GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure2);
}
*/

void InitAF_sajat_motor()
{
	//PWM lábak a fet driverek irányításához
	GPIO_InitTypeDef  GPIO_InitStructure;
	__GPIOA_CLK_ENABLE();
	// GPIO alternate function lábak
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;

	GPIO_InitStructure.Pin =  GPIO_PIN_9 | GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);


	//Engedélyező láb a fet driverekhez

	__GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStructure2;
	GPIO_InitStructure2.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure2.Pull = GPIO_NOPULL;
	GPIO_InitStructure2.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure2.Pin =  GPIO_PIN_1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure2);
}


void enable_fet_drivers(uint8_t enable){
	if(enable){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

	}
}


//
void Init_gyari_motor_PWM()
{
	__TIM1_CLK_ENABLE();
	//168 MHz
	Tim1Handle.Instance = TIM1;
	// Timer1 konfigurációja PWM üzemmódban
	Tim1Handle.Instance = TIM1;
	Tim1Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim1Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim1Handle.Init.Prescaler = 41;
	Tim1Handle.Init.Period = 65480;
	Tim1Handle.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&Tim1Handle);
	HAL_TIM_PWM_Init(&Tim1Handle);

	TIM_OC_InitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 0;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_LOW;
	TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;

//		HAL_TIM_PWM_ConfigChannel(&Tim1Handle, &TIM_OCInitStructure, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&Tim1Handle, &TIM_OCInitStructure, TIM_CHANNEL_2);

//		HAL_TIM_PWM_Start(&Tim4Handle, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&Tim1Handle, TIM_CHANNEL_2);

	Tim1Handle.Instance->CCR2 = GYARI_MOTOR_COUNTER_KOZEP;
//	Tim1Handle.Instance->CCR2 = 6500;
}

void Init_sajat_motor_PWM()
{

	//TODO
	// make a function for TIM1 init
	__TIM1_CLK_ENABLE();
	//TIM1 APB2-n, így 168MHz
	Tim1Handle.Instance = TIM1;
	// Timer1 konfigurációja PWM üzemmódban
	Tim1Handle.Instance = TIM1;
	Tim1Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim1Handle.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
//	Tim1Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim1Handle.Init.Prescaler = 0;
	Tim1Handle.Init.Period = SAJAT_MOTOR_COUNTER_MAX;
	Tim1Handle.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&Tim1Handle);
	HAL_TIM_PWM_Init(&Tim1Handle);

	TIM_OC_InitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 0;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_LOW;
	TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;

	HAL_TIM_PWM_ConfigChannel(&Tim1Handle, &TIM_OCInitStructure, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&Tim1Handle, &TIM_OCInitStructure, TIM_CHANNEL_3);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_TIM_PWM_Start(&Tim1Handle, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&Tim1Handle, TIM_CHANNEL_3);
	Tim1Handle.Instance->CCR2 = 0;
	Tim1Handle.Instance->CCR3 = 0;
}

void set_compare_value_sajat_motor(uint16_t value)
{
	Tim1Handle.Instance->CCR2 = value;
	Tim1Handle.Instance->CCR3 = SAJAT_MOTOR_COUNTER_MAX - value;
}


void InitTimer()
{
	// APB1: 42 MHz (TIMx: 84 MHz)
	__TIM7_CLK_ENABLE();

	// TIM7: 10 kHz (TIM7_FREQ)
	Tim7Handle.Instance = TIM7;
	Tim7Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim7Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim7Handle.Init.Prescaler = 1259;
	Tim7Handle.Init.Period = 999;
	Tim7Handle.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&Tim7Handle);

	HAL_NVIC_SetPriority(TIM7_IRQn, 3, 3);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_TIM_Base_Start_IT(&Tim7Handle);
}

/* Megszakításkezelő a TIM7 időzítőhöz */


/* Megszakításkezelő callback a Timer megszakításkezelőhöz */
uint direction = 0;

void TimerHandler() {

	/*
	if(Tim4Handle.Instance->CCR1 <= DIGIT_SZ_MAX_CMP && Tim4Handle.Instance->CCR1 >= DIGIT_SZ_MIN_CMP && direction == 0)
	{
		Tim4Handle.Instance->CCR1++;
		if(Tim4Handle.Instance->CCR1 >= DIGIT_SZ_MAX_CMP - 100){
			direction = 1;
		}
	} else {
		Tim4Handle.Instance->CCR1--;
		if(Tim4Handle.Instance->CCR1 <= DIGIT_SZ_MIN_CMP +100){
			direction = 0;
		}
	}
	*/

	if(Tim1Handle.Instance->CCR2 <= 6560 && Tim1Handle.Instance->CCR2 >= 6150 && direction == 0)
	{
		Tim1Handle.Instance->CCR2 = Tim1Handle.Instance->CCR2 + 10;
		if(Tim1Handle.Instance->CCR2 >= 6560){
			//direction = 1;
		}
	} else {
		Tim1Handle.Instance->CCR2--;
		if(Tim1Handle.Instance->CCR2 <= 6150){
			direction = 0;
		}
	}

}


void set_gyari_motor_compare_value(uint16_t value){
	if(value > GYARI_MOTOR_COUNTER_MAX)
	{
		Tim1Handle.Instance->CCR2 = GYARI_MOTOR_COUNTER_MAX;
	} else if(value < GYARI_MOTOR_COUNTER_MIN)
	{
		Tim1Handle.Instance->CCR2 = GYARI_MOTOR_COUNTER_MIN;
	} else {
		Tim1Handle.Instance->CCR2 = value;
	}
}

