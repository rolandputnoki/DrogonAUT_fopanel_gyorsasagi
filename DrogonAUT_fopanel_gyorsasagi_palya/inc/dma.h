/*
 * dma.h
 *
 *  Created on: 2017. nov. 18.
 *      Author: Patrik
 */

#ifndef DMA_H_
#define DMA_H_

#include "stm32f4xx.h"

void DMA_Init(void);



//ADC DMA kezelõk
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
DMA_HandleTypeDef hdma_adc3;

#endif /* DMA_H_ */
