#include "bsp_spi.h"
#include "bsp.h"

//SPI_HandleTypeDef spi;


void SPI1_init(){
	__GPIOA_CLK_ENABLE();
		__SPI1_CLK_ENABLE();

		spi.Instance = SPI1;
		spi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
		spi.Init.Direction = SPI_DIRECTION_2LINES;
		spi.Init.CLKPhase = SPI_PHASE_2EDGE;
		spi.Init.CLKPolarity = SPI_POLARITY_LOW;
		spi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
		spi.Init.DataSize = SPI_DATASIZE_8BIT;
		spi.Init.FirstBit = SPI_FIRSTBIT_MSB;
		spi.Init.NSS = SPI_NSS_HARD_OUTPUT;
		spi.Init.TIMode = SPI_TIMODE_DISABLED;
		spi.Init.Mode = SPI_MODE_MASTER;
		if (HAL_SPI_Init(&spi) != HAL_OK)
		{
			asm("bkpt 255");
		}

		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = GPIO_PIN_15;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitTypeDef GPIO_InitStruct_kettes_spi;
		GPIO_InitStruct_kettes_spi.Pin = GPIO_PIN_8;
		GPIO_InitStruct_kettes_spi.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct_kettes_spi.Pull = GPIO_NOPULL;
		GPIO_InitStruct_kettes_spi.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct_kettes_spi);

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

	//		HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
		HAL_NVIC_SetPriority(SPI1_IRQn, 0,0);
		HAL_NVIC_EnableIRQ(SPI1_IRQn);
}


void SPI1_IRQHandler(void)
{
	HAL_SPI_IRQHandler(&spi);
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *handle){
	if(handle->Instance == SPI1){
		data_received = 1;

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	}
}


void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef * hspi){
	if(hspi->Instance == SPI1){
		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	}
}
