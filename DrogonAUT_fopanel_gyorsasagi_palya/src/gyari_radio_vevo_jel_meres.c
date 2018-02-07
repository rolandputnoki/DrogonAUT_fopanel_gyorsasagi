/*
 * gyari_radio_vevo_jel_meres.c
 *
 *  Created on: 2017. okt. 29.
 *      Author: Roland
 */
#include <motor_pwm.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

#include "uart_communication.h"
#include "infra_receiver.h"
#include "rc5.h"



/***********************************/
/* Körforgalom jelvétel */
extern uint8_t korforgalom_uzenet = 0;
extern uint8_t korforgalom_cim_stimmel = 0;
/***********************************/



uint16_t ic_tomb[500];

extern uint16_t capture_ertek = 0;

//PA11-re kÃ¶tjÃ¼k a RC vevÅ‘ motor csatornÃ¡jÃ¡t
void set_gy_rv_af_motor(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	__GPIOA_CLK_ENABLE();
	// GPIO alternate function lÃ¡bak
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure.Pin =  GPIO_PIN_11;
	GPIO_InitStructure.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//PB15-re kÃ¶tjÃ¼k a RC vevÅ‘ szervo csatornÃ¡jÃ¡t
void set_gy_rv_af_szervo(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	__GPIOB_CLK_ENABLE();
	// GPIO alternate function lÃ¡bak
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure.Pin =  GPIO_PIN_15;
	GPIO_InitStructure.Alternate = GPIO_AF9_TIM12;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}



TIM_IC_InitTypeDef TIM_ICInitStructure;
TIM_HandleTypeDef Tim12Handle;

void Init_input_capture_motor()
{
	/*
	__TIM1_CLK_ENABLE();
	//TIM1 APB2-n, Ã­gy 168MHz
	Tim1Handle.Instance = TIM1;
	// Timer1 konfigurÃ¡ciÃ³ja PWM Ã¼zemmÃ³dban
	Tim1Handle.Instance = TIM1;
	Tim1Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim1Handle.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
	//	Tim1Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim1Handle.Init.Prescaler = 0;
	Tim1Handle.Init.Period = SAJAT_MOTOR_COUNTER_MAX;
	Tim1Handle.State = HAL_TIM_STATE_RESET;

	HAL_TIM_Base_Init(&Tim1Handle);
*/

	//TIM1 mÃ¡r konfigurÃ¡lva van a motorvezÃ©rlÃ©shez
	HAL_TIM_IC_Init(&Tim1Handle);

	TIM_ICInitStructure.ICFilter = 15;
	TIM_ICInitStructure.ICSelection	= TIM_ICSELECTION_DIRECTTI;
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;
	TIM_ICInitStructure.ICPrescaler = TIM_ETRPRESCALER_DIV1;
	HAL_TIM_IC_ConfigChannel(&Tim1Handle, &TIM_ICInitStructure, TIM_CHANNEL_4);

	HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
	Tim1Handle.Instance->CCR4 = 0;
	HAL_TIM_IC_Start_IT(&Tim1Handle, TIM_CHANNEL_4);
}


void Init_input_capture_szervo()
{
	__TIM12_CLK_ENABLE();

	//TIM12 84 MHz
	Tim12Handle.Instance = TIM12;
	Tim12Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim12Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim12Handle.Init.Prescaler = 21;
	Tim12Handle.Init.Period = 65000;
	Tim12Handle.State = HAL_TIM_STATE_RESET;


	HAL_TIM_Base_Init(&Tim12Handle);
	HAL_TIM_IC_Init(&Tim12Handle);

	TIM_ICInitStructure.ICFilter = 15;
	TIM_ICInitStructure.ICSelection	= TIM_ICSELECTION_DIRECTTI;
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;
	TIM_ICInitStructure.ICPrescaler = TIM_ETRPRESCALER_DIV1;
	HAL_TIM_IC_ConfigChannel(&Tim12Handle, &TIM_ICInitStructure, TIM_CHANNEL_2);

	HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
	Tim12Handle.Instance->CCR2 = 0;
	HAL_TIM_IC_Start_IT(&Tim12Handle, TIM_CHANNEL_2);
}

uint8_t rising_edge = 1;
uint16_t cmp1, cmp2, cmp;

uint16_t ic_count = 0;



//RC5 protokollhoz

uint8_t first_edge = 1;
uint8_t bits_in_message[28];
uint8_t bit_counter = 0;
uint8_t cur_bit_is_high;
uint8_t communication_is_ok = 1;

uint8_t falling_edge = 0;

RC5STATE protocol_state;
RC5STATE new_protocol_state;
uint8_t first_rising_edge = 1;
uint8_t cur_is_rising_edge = 0;
uint16_t prev_edge_cnt, cur_edge_cnt;
uint16_t pulse_lenght;
RC5EVENT cur_event;
uint8_t rc5_message[14];
uint8_t msg_pos_counter = 1;
uint8_t rc5_transfer_finished = 0;

uint16_t message = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{

	if(htim->Instance == TIM1)
	{

		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4 )
		{


			if(rising_edge){
				cmp1 = htim->Instance->CCR4;
				rising_edge = 0;
				TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_FALLING;
				__HAL_TIM_SetCounter(&Tim1Handle, 0);
				HAL_TIM_IC_ConfigChannel(&Tim1Handle, &TIM_ICInitStructure, TIM_CHANNEL_4);
				HAL_TIM_IC_Start_IT(&Tim1Handle, TIM_CHANNEL_4);

			} else {
				rising_edge = 1;
				TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;
				HAL_TIM_IC_ConfigChannel(&Tim1Handle, &TIM_ICInitStructure, TIM_CHANNEL_4);
				HAL_TIM_IC_Start_IT(&Tim1Handle, TIM_CHANNEL_4);
				cmp2 = htim->Instance->CCR4;
				capture_ertek = cmp2 - cmp1;

				if(capture_ertek < 6000){
					motor_value = 6200;
				} else {
					motor_value = capture_ertek;
				}
			}
		}
	} else if(htim->Instance == TIM12)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 )
		{
			cmp = htim->Instance->CCR2;
			Tim4Handle.Instance->CCR1 = cmp;
			if(rising_edge){
				rising_edge = 0;
				TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_FALLING;
				__HAL_TIM_SetCounter(&Tim12Handle, 0);
				HAL_TIM_IC_ConfigChannel(&Tim12Handle, &TIM_ICInitStructure, TIM_CHANNEL_2);
				HAL_TIM_IC_Start_IT(&Tim12Handle, TIM_CHANNEL_2);

			} else {
				rising_edge = 1;
				TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;
				HAL_TIM_IC_ConfigChannel(&Tim12Handle, &TIM_ICInitStructure, TIM_CHANNEL_2);
				HAL_TIM_IC_Start_IT(&Tim12Handle, TIM_CHANNEL_2);
			}
		}
	}
	
	else if(htim->Instance == TIM3)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
		{


			if(communication_is_ok) {

				cur_bit_is_high = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);

				if(first_edge){
					htim->Instance->CNT = 0;
					first_edge = 0;
					bits_in_message[0] = 1;
					bit_counter++;


					if(cur_bit_is_high){
						//Az elsõ él mindig lefutó él kell, hogy legyen. Ezért ha itt egyes, akkor felfutó él volt, tehát hiba van.
						communication_is_ok = 0;
					}
				} else {

					pulse_lenght = htim->Instance->CNT;
					htim->Instance->CNT = 0;

					if(pulse_lenght >  50){
						bits_in_message[0] = 1;
						bit_counter = 1;

					}

					if(pulse_lenght > 10 && pulse_lenght < 25){
						if(cur_bit_is_high){
							bits_in_message[bit_counter] = 0;
							bit_counter++;

							//Kiegészítés, mert már nem lesz több él, de 1 fél bit még kimaradt
							if(bit_counter == 27){
								bits_in_message[bit_counter] = 1;
								bit_counter++;
							}
						} else {
							bits_in_message[bit_counter] = 1;
							bit_counter++;
						}
					}

					if(pulse_lenght > 26 && pulse_lenght < 45){
						if(cur_bit_is_high){
							bits_in_message[bit_counter] = 0;
							bit_counter++;
							bits_in_message[bit_counter] = 0;
							bit_counter++;

							//Kiegészítés, mert már nem lesz több él, de 1 fél bit még kimaradt
							if(bit_counter == 27){
								bits_in_message[bit_counter] = 1;
								bit_counter++;
							}
						} else {
							bits_in_message[bit_counter] = 1;
							bit_counter++;
							bits_in_message[bit_counter] = 1;
							bit_counter++;
						}
					}

					if(bit_counter >= 28){
						korforgalom_uzenet = bits_in_message[22]*4 + bits_in_message[24]*2 + bits_in_message[26]*1;


//						uint8_t uzenet_cim = bits_in_message[8]*8 + bits_in_message[10]*4 + bits_in_message[12]*2 + bits_in_message[14]*1;
						if((bits_in_message[8]*8 + bits_in_message[10]*4 + bits_in_message[12]*2 + bits_in_message[14]*1) == 13){
							korforgalom_cim_stimmel = 1;
							HAL_NVIC_DisableIRQ(TIM3_IRQn);
						}


						//Teszthez
/*
						char buffferem[6];
						itoa(korforgalom_uzenet, buffferem, 10);
						BT_UART_SendString(buffferem);
*/

						bit_counter = 0;
						first_edge = 1;
					}
				}


			} else {
				pulse_lenght = htim->Instance->CNT;
				htim->Instance->CNT = 0;

				if(pulse_lenght > 50)
				{
					communication_is_ok = 1;
					first_edge = 0;
					bit_counter = 1;
					bits_in_message[0] = 1;
				}
			}




		}
	}
}


void TIM1_CC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&Tim1Handle);
}

void TIM8_BRK_TIM12_IRQHandler(void){
	HAL_TIM_IRQHandler(&Tim12Handle);
}



