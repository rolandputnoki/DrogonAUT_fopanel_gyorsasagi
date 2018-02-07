/*
 * bsp_spi.h
 *
 *  Created on: 2017. nov. 5.
 *      Author: Dominik
 */

#ifndef BSP_SPI_H_
#define BSP_SPI_H_


#include "bsp.h"

HAL_StatusTypeDef SPI_Init();

void SPI_EnableChip(int enable);

HAL_StatusTypeDef SPI_Send(uint8_t* pData, uint16_t dataSize);

HAL_StatusTypeDef SPI_Receive(uint8_t* pData, uint16_t dataSize);

HAL_StatusTypeDef SPI_SendReceive(uint8_t* pDataIn, uint8_t *pDataOut, uint16_t dataSize);


extern SPI_HandleTypeDef spi;
extern uint8_t data_received;


#endif /* BSP_SPI_H_ */
