/** TODO 2. I2C feladat
 * Az I2C kommunikációs feladat megoldást ebben a fájlban készítse el.
 * Az itt elkészült függvények elsődleges feladata a thermometer.c kommunikációs funkcióinak kiszolgálása.
 *
 * Ehhez egyrészt inicializálni kell az I2C kommunikációt mind a processzor lábainak megfelelő beállításával,
 * mind az I2C1 periféria megfelelő beállításával. (HAL_I2C_MspInit() és I2C_Init() függvények.)
 *
 * Ezután a I2C_ReadRegister() és I2C_WriteRegister() felelősek egy I2C periféria egy adott regiszterének írásáért vagy
 * olvasásáért. Ebben a HAL nagyon sokat segít nekünk. A háttérben lévő kommunikáció tartalmaz egy start conditiont, a
 * periféria megcímzését, a kért regiszter címének elküldését, majd a tényleges adatok küldését vagy fogadását.
 * Olvasáskor külön komplikációt jelent, hogy először írni kell (a kért regiszter címét), utána pedig olvasni, amit az
 * I2C protokoll keretein belül egy restart condition és egy újracímzés segítségével lehet megoldni.
 * Ezt a HAL mind megoldja helyettünk.
 * */

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"


I2C_HandleTypeDef hi2c;

const uint32_t i2c_timeout = 5000;

HAL_StatusTypeDef I2C_Init(uint32_t ownAddress)
{
	  hi2c.Instance = I2C3;
	  hi2c.Init.ClockSpeed = 400000;
	  hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
	  hi2c.Init.OwnAddress1 = ownAddress;
	  hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  return HAL_I2C_Init(&hi2c);
}


HAL_StatusTypeDef I2C_ReadRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize)
{
	return HAL_I2C_Mem_Read(&hi2c,deviceAddress, registerAddress, 1, pData, dataSize, i2c_timeout);
}


HAL_StatusTypeDef I2C_WriteRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize)
{
	return HAL_I2C_Mem_Write(&hi2c, deviceAddress, (uint16_t) registerAddress, 1, pData, dataSize, i2c_timeout);
}


void HAL_I2C_MspInit(I2C_HandleTypeDef *handle)
{
	GPIO_InitTypeDef GPIO_InitStructA;
	GPIO_InitTypeDef GPIO_InitStructC;
	if(handle->Instance==I2C3)
	{

		__GPIOA_CLK_ENABLE();
		__GPIOC_CLK_ENABLE();

		/**I2C3 GPIO Configuration
		PC9     ------> I2C3_SDA
		PA8     ------> I2C3_SCL
		*/
		GPIO_InitStructC.Pin = GPIO_PIN_9;
		GPIO_InitStructC.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStructC.Pull = GPIO_PULLUP;
		GPIO_InitStructC.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructC.Alternate = GPIO_AF4_I2C3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructC);

		GPIO_InitStructA.Pin = GPIO_PIN_8;
		GPIO_InitStructA.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStructA.Pull = GPIO_PULLUP;
		GPIO_InitStructA.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructA.Alternate = GPIO_AF4_I2C3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructA);

		__I2C3_CLK_ENABLE();
	}
}
