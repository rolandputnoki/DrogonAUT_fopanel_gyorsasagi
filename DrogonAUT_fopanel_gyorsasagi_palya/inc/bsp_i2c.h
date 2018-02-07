/**
*****************************************************************************
** Kommunikációs mérés - i2c.h
** I2C kommunikáció
*****************************************************************************
*/
#pragma once
#ifndef _I2C_H__
#define _I2C_H__
#include "stm32f4xx_hal.h"


HAL_StatusTypeDef I2C_Init(uint32_t ownAddress);

HAL_StatusTypeDef I2C_ReadRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize);

HAL_StatusTypeDef I2C_WriteRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize);

#endif
