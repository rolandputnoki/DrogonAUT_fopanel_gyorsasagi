/*
 * dma.c
 *
 *  Created on: 2017. nov. 18.
 *      Author: Patrik
 */

#include "dma.h"
#include "adc.h"



void ConfigureDMA()
{
	__DMA2_CLK_ENABLE();


	//ADC1
	hdma_adc1.Instance = DMA2_Stream4;
	hdma_adc1.Init.Channel  = DMA_CHANNEL_0;
	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_adc1.Init.Mode = DMA_CIRCULAR;
	hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;

	HAL_DMA_Init(&hdma_adc1);

	__HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);


	//ADC2
	hdma_adc2.Instance = DMA2_Stream2;
	hdma_adc2.Init.Channel  = DMA_CHANNEL_1;
	hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_adc2.Init.Mode = DMA_CIRCULAR;
	hdma_adc2.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_adc2.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_adc2.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	hdma_adc2.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_adc2.Init.PeriphBurst = DMA_PBURST_SINGLE;

	HAL_DMA_Init(&hdma_adc2);

	__HAL_LINKDMA(&hadc2, DMA_Handle, hdma_adc2);

	//ADC2
	hdma_adc3.Instance = DMA2_Stream0;
	hdma_adc3.Init.Channel  = DMA_CHANNEL_2;
	hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_adc3.Init.Mode = DMA_CIRCULAR;
	hdma_adc3.Init.Priority = DMA_PRIORITY_HIGH;
	hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	hdma_adc3.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	hdma_adc3.Init.MemBurst = DMA_MBURST_SINGLE;
	hdma_adc3.Init.PeriphBurst = DMA_PBURST_SINGLE;

	HAL_DMA_Init(&hdma_adc3);

	__HAL_LINKDMA(&hadc3, DMA_Handle, hdma_adc3);

	//ADC1
	HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

	//ADC2
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

	//ADC3
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}


void DMA2_Stream4_IRQHandler()
{
    HAL_DMA_IRQHandler(&hdma_adc1);
}


void DMA2_Stream2_IRQHandler()
{
    HAL_DMA_IRQHandler(&hdma_adc2);
}

void DMA2_Stream0_IRQHandler()
{
    HAL_DMA_IRQHandler(&hdma_adc3);
}
