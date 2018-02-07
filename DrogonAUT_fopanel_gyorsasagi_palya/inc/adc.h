/*
 * ADC.h
 *
 *  Created on: 2017. okt. 30.
 *      Author: Patrik
 */

#ifndef ADC_H_
#define ADC_H_

#include "stm32f4xx.h"



//Az ADC1 14-es csatornáján olvassuk a PC4-n elhelyezkedõ 4-30 cm-es szenzort
//AZ ADC2 15-es csatornáján olvassuk a PC5-n elhelyezkedõ 4-30 cm-es szenzort
//AZ ADC3 10-es csatornáján olvassuk a PC0-n elhelyezkedõ 20-150 cm-es szenzort
#define ADC1_READ_CH ADC_CHANNEL_14
#define ADC2_READ_CH ADC_CHANNEL_15
#define ADC3_READ_CH ADC_CHANNEL_10



//ADC kezelõk
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;


void ADC_Init();

void ADC1_Init(void);
void ADC2_Init(void);
void ADC3_Init(void);

#endif /* ADC_H_ */
