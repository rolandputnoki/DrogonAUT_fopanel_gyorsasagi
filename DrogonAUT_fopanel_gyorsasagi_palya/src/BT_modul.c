/*
 * BT_modul.c
 *
 *  Created on: 2017. okt. 29.
 *      Author: Roland
 */
#include "uart_communication.h"
#include "BT_modul.h"


//	BT_UART_SendString("AT+AB AutoReconnectSetup 1 2000 00c2c675c28f SPP\r\n");
//	BT_UART_SendString("AT+AB AutoReconnect enable\r\n");


	/*
	if(!is_connection_up()){
		BT_modul_init();
	}

	*/



uint8_t counter = 0;
char answer[100];
char answer_command[18];

uint8_t is_connection_up(){
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10) == GPIO_PIN_RESET){
		return 0;
	} else {
		return 1;
	}
}

void BT_init_pins(){

	__GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = BT_Con_Status_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(BT_Con_Status_Port, &GPIO_InitStructure);


	/*
	__GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	*/
}

void BT_modul_init(){
	BT_UART_SendString("AT+AB ShowConnection\r\n");
//	HAL_Delay(1);
	while(!is_connection_up())
	{
		BT_UART_SendString("AT+AB SPPConnect 00c2c675c28f\r\n");
		while(!answer_received)
		{
			if(is_connection_up()){
				break;
			}
		}

		while(counter < last_received_message_size){
			answer[counter] = lastReceivedMessage[counter];
			counter++;
		}
		counter = 0;
		answer_received = 0;
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	BT_UART_SendString("Connected\r\n");
}




