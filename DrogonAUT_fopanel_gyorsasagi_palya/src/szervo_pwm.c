/*
 * gyari_szervo_pwm.c
 *
 *  Created on: 2017. okt. 29.
 *      Author: Roland
 */
#include <szervo_pwm.h>

TIM_HandleTypeDef Tim4Handle;

// Alternate function GPIO lábak inicializálása
void Init_AF_szervo()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	__GPIOB_CLK_ENABLE();
	// GPIO alternate function lábak
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure.Pin =  GPIO_PIN_6;
	GPIO_InitStructure.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}


//
void Init_GY_Szervo_PWM()
{
	__TIM4_CLK_ENABLE();
	Tim4Handle.Instance = TIM4;
	//84MHz
	// Timer4 konfigurációja PWM üzemmódban
	Tim4Handle.Instance = TIM4;
	Tim4Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim4Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim4Handle.Init.Prescaler = 21;
	Tim4Handle.Init.Period = 62426;
	Tim4Handle.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&Tim4Handle);
	HAL_TIM_PWM_Init(&Tim4Handle);

	TIM_OC_InitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 0;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_LOW;
	TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;

	HAL_TIM_PWM_ConfigChannel(&Tim4Handle, &TIM_OCInitStructure, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&Tim4Handle, TIM_CHANNEL_1);

	Tim4Handle.Instance->CCR1 = 5727;
}

void Init_Digit_Szervo_PWM()
{

	//TIM4 az APB1 buszra csatlakozik, így az órajele 84MHz
	__TIM4_CLK_ENABLE();
	Tim4Handle.Instance = TIM4;
	// Timer4 konfigurációja PWM üzemmódban
	Tim4Handle.Instance = TIM4;
	Tim4Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim4Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim4Handle.Init.Prescaler = 3;
	Tim4Handle.Init.Period = 63062;
	Tim4Handle.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&Tim4Handle);
	HAL_TIM_PWM_Init(&Tim4Handle);

	TIM_OC_InitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 0;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_LOW;
	TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;

	HAL_TIM_PWM_ConfigChannel(&Tim4Handle, &TIM_OCInitStructure, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&Tim4Handle, TIM_CHANNEL_1);

	Tim4Handle.Instance->CCR1 = 31500;
}

void Init_sajat_Szervo_PWM()
{
	//TIM4 az APB1 buszra csatlakozik, így az órajele 84MHz
		__TIM4_CLK_ENABLE();
		Tim4Handle.Instance = TIM4;
		// Timer4 konfigurációja PWM üzemmódban
		Tim4Handle.Instance = TIM4;
		Tim4Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		Tim4Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
		Tim4Handle.Init.Prescaler = 25;
		Tim4Handle.Init.Period = 64514;
		Tim4Handle.State = HAL_TIM_STATE_RESET;

		HAL_TIM_Base_Init(&Tim4Handle);
		HAL_TIM_PWM_Init(&Tim4Handle);

		TIM_OC_InitTypeDef TIM_OCInitStructure;
		TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
		TIM_OCInitStructure.Pulse = 0;
		TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
		TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_LOW;
		TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
		TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
		TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;

		HAL_TIM_PWM_ConfigChannel(&Tim4Handle, &TIM_OCInitStructure, TIM_CHANNEL_1);

		HAL_TIM_PWM_Start(&Tim4Handle, TIM_CHANNEL_1);

		Tim4Handle.Instance->CCR1 = 4846;
}
void set_compare_value_gy_szervo(uint16_t value){

	if(value > GYARI_SZ_MAX_CMP)
	{
		Tim4Handle.Instance->CCR1 = GYARI_SZ_MAX_CMP;
	} else if(value < GYARI_SZ_MIN_CMP)
	{
		Tim4Handle.Instance->CCR1 = GYARI_SZ_MIN_CMP;
	} else {
		Tim4Handle.Instance->CCR1 = value;
	}
}

void set_compare_value_sajat_szervo(uint16_t value){

	if(value > SAJAT_SZ_MAX_CMP)
	{
		Tim4Handle.Instance->CCR1 = SAJAT_SZ_MAX_CMP;
	} else if(value < SAJAT_SZ_MIN_CMP)
	{
		Tim4Handle.Instance->CCR1 = SAJAT_SZ_MIN_CMP;
	} else {
		Tim4Handle.Instance->CCR1 = value;
	}
}

void set_compare_value_digit_szervo(uint16_t value){

	if(value > DIGIT_SZ_MAX_CMP)
	{
		Tim4Handle.Instance->CCR1 = DIGIT_SZ_MAX_CMP;
	} else if(value < DIGIT_SZ_MIN_CMP)
	{
		Tim4Handle.Instance->CCR1 = DIGIT_SZ_MIN_CMP;
	} else {
		Tim4Handle.Instance->CCR1 = value;
	}
}


