/*
 * encoder.c
 *
 *  Created on: 2018. jan. 6.
 *      Author: Roland
 */
#include "encoder.h"
#include "bsp.h"

TIM_HandleTypeDef htim2;
TIM_Encoder_InitTypeDef encoder;

uint32_t ic1, ic2;



void TIM2_IRQHandler(void){
	HAL_TIM_IRQHandler(&htim2);
}


void encoder_init()
{

	GPIO_InitTypeDef GPIO_InitStruct;


    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 0;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 0xFFFFFFFF;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    if(htim2.Instance == TIM2)
    {
    	__TIM2_CLK_ENABLE();
    	__GPIOA_CLK_ENABLE();

    	//Pinek beállítása
        GPIO_InitStruct.Pin = ENCODER_CH_A_PIN | ENCODER_CH_B_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    	HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
	    HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }


    encoder.EncoderMode = TIM_ENCODERMODE_TI12;

    encoder.IC1Filter = 0x0F;
//    encoder.IC1Polarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
    encoder.IC1Polarity = TIM_INPUTCHANNELPOLARITY_FALLING;
    encoder.IC1Prescaler = TIM_ICPSC_DIV4;
    encoder.IC1Selection = TIM_ICSELECTION_DIRECTTI;

    encoder.IC2Filter = 0x0F;
    //    encoder.IC1Polarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
    encoder.IC2Polarity = TIM_INPUTCHANNELPOLARITY_RISING;
    encoder.IC2Prescaler = TIM_ICPSC_DIV4;
    encoder.IC2Selection = TIM_ICSELECTION_DIRECTTI;

    if (HAL_TIM_Encoder_Init(&htim2, &encoder) != HAL_OK) {
    	Error_Handler();
    }

    //DMA-val
/*
    if( HAL_TIM_Encoder_Start_DMA(&htim2, TIM_CHANNEL_ALL, &ic1, &ic2, 8 ) != HAL_OK){
    	Error_Handler();
    }
*/


    //Interrupt-tal

    if(HAL_TIM_Encoder_Start_IT(&htim2,TIM_CHANNEL_ALL)!=HAL_OK){
    	Error_Handler();
    }


}




uint32_t get_encoder_counter()
{
	return htim2.Instance->CNT;
}

void Error_Handler()
{
	uint8_t baj_van;
}





