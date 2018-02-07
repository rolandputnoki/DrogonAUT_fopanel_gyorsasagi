#include "lsm6ds3.h"
#include "bsp_i2c.h"
#include "LSM6DS3_ACC_GYRO_driver_HL.h"
#include "stm32f4xx_hal_def.h"

DrvContextTypeDef testInit;


DrvStatusTypeDef LSM6DS3_Init(){
	if(I2C_Init(0x00) == HAL_OK){
		testInit.who_am_i = LSM6DS3_ACC_GYRO_WHO_AM_I;
		testInit.ifType = 0;
		testInit.address = LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW;
		testInit.isEnabled = 1;
		return LSM6DS3_X_Init(&testInit);
	}
	else return COMPONENT_ERROR;
}

DrvStatusTypeDef LMS6DS3_Read_Acc(){
	return COMPONENT_OK;//LSM6DS3_X_Drv.Get_Axes(&testInit, &axesValues);
}

int LSM6DS3_Init2(){
	uint8_t pData;
	uint8_t initData;
	uint8_t DataToWrite;
	uint8_t Data[2];

	DataToWrite = 0xE4;

	if(I2C_WriteRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DS3_ACC_GYRO_CTRL8_XL, &DataToWrite, 1) == HAL_OK)
	{
		uint8_t ittvagyok = 0;
	}
	HAL_Delay(10);


	DataToWrite = 0x10;

	if(I2C_WriteRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DS3_ACC_GYRO_TAP_CFG1, &DataToWrite, 1) == HAL_OK)
	{
		uint8_t ittvagyok = 0;
	}
	HAL_Delay(10);


	DataToWrite = 0x80;

	if(I2C_WriteRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, 	LSM6DS3_ACC_GYRO_CTRL4_C, &DataToWrite, 1) == HAL_OK)
	{
		uint8_t ittvagyok = 0;
	}
	HAL_Delay(10);


	DataToWrite = 0x53;

	if(I2C_WriteRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, 	LSM6DS3_ACC_GYRO_CTRL1_XL, &DataToWrite, 1) == HAL_OK)
	{
		uint8_t ittvagyok = 0;
	}
	HAL_Delay(10);


	DataToWrite = 0x50;

	if(I2C_WriteRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, 	LSM6DS3_ACC_GYRO_CTRL2_G, &DataToWrite, 1) == HAL_OK)
	{
		uint8_t ittvagyok = 0;
	}
	HAL_Delay(10);




/*
	int res;
	if(I2C_ReadRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DS3_ACC_GYRO_WHO_AM_I_REG, &pData,1 ) == HAL_OK){
		if(pData == 0x69){
			initData = 0x48;
			res = I2C_WriteRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DS3_ACC_GYRO_CTRL1_XL, &initData, 1 );
			HAL_Delay(5);
			return 0;
		}
	}

*/




	return 1;
}

int LMS6DS3_Read_Axes(int32_t *x_axis, int32_t *y_axis, int32_t *z_axis){
	uint8_t regValue[6];

	if(I2C_ReadRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DS3_ACC_GYRO_OUTX_L_XL,  &regValue[0] ,2 ) != HAL_OK){
		return 1;
	}
	if(I2C_ReadRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DS3_ACC_GYRO_OUTY_L_XL, &regValue[2] ,2 ) != HAL_OK){
		return 1;
	}
	if(I2C_ReadRegister(LSM6DS3_ACC_GYRO_I2C_ADDRESS_LOW, LSM6DS3_ACC_GYRO_OUTZ_L_XL, &regValue[4] ,2 ) != HAL_OK){
		return 1;
	}

	int16_t pData[3];

	pData[0] = ( ( ( ( int16_t )regValue[1] ) << 8 ) | ( int16_t )regValue[0] );
	pData[1] = ( ( ( ( int16_t )regValue[3] ) << 8 ) | ( int16_t )regValue[2] );
	pData[2] = ( ( ( ( int16_t )regValue[5] ) << 8 ) | ( int16_t )regValue[4] );


	/* Calculate the data. */
	*x_axis = ( int32_t )( pData[0] * LSM6DS3_ACC_SENSITIVITY_FOR_FS_2G );
	*y_axis = ( int32_t )( pData[1] * LSM6DS3_ACC_SENSITIVITY_FOR_FS_2G );
	*z_axis = ( int32_t )( pData[2] * LSM6DS3_ACC_SENSITIVITY_FOR_FS_2G );

	return 0;
}
