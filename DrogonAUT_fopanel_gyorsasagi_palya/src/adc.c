/*
 * ADC.c
 *
 *  Created on: 2017. okt. 30.
 *      Author: Patrik
 */

#include "adc.h"
#include "bsp.h"

//mindhárom ADC inicializálásnak hívása egy függvénnyel
void ADC_Init()
{
	ADC_init_pins();
	ADC1_Init();
	ADC2_Init();
	ADC3_Init();
}


void ADC_init_pins(){
	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOC_CLK_ENABLE();
   GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_4|GPIO_PIN_5;
   GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


void ADC1_Init()
{
	__ADC1_CLK_ENABLE();

    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    ADC_ChannelConfTypeDef adcChannel;

    hadc1.Instance = ADC1;

    hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_10B;
    hadc1.Init.ScanConvMode = DISABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.NbrOfDiscConversion = 0;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 1;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = DISABLE;

    HAL_ADC_Init(&hadc1);

    adcChannel.Channel = ADC1_READ_CH;
    adcChannel.Rank = 1;
    adcChannel.SamplingTime = ADC_SAMPLETIME_480CYCLES;
    adcChannel.Offset = 0;

    if (HAL_ADC_ConfigChannel(&hadc1, &adcChannel) != HAL_OK)
    {
        asm("bkpt 255");
    }
}


//ADC2 inicilizálása
//single mód --> egy lábon lévõ konverzió
//nem folytonos mód, csak híváskor konvertál
//DMA mód használata
void ADC2_Init()
{

  __ADC2_CLK_ENABLE();
  ADC_ChannelConfTypeDef sConfig;

  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_10B;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.NbrOfDiscConversion = 0;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = DISABLE;


  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

  sConfig.Channel = ADC2_READ_CH;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;			//órajelbõl és adatlapi konstansból számolva
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    //_Error_Handler(__FILE__, __LINE__);
  }

}

//ADC3 inicilizálása
//single mód --> egy lábon lévõ konverzió
//nem folytonos mód, csak híváskor konvertál
//DMA mód használata
void ADC3_Init(void)
{

  __ADC3_CLK_ENABLE();
  ADC_ChannelConfTypeDef sConfig;

  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_10B;
  hadc3.Init.ScanConvMode = DISABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.NbrOfDiscConversion = 0;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = DISABLE;



  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
   // _Error_Handler(__FILE__, __LINE__);
  }

  sConfig.Channel = ADC3_READ_CH;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)		//órajelbõl és adatlapi konstansból számolva
  {
   // _Error_Handler(__FILE__, __LINE__);
  }
}


void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  HAL_ADC_IRQHandler(&hadc3);
}



