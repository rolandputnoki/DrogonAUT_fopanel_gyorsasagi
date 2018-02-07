/*
 * bsp.h
 *
 *  Created on: 2017. okt. 30.
 *      Author: Roland
 */

#ifndef BSP_H_
#define BSP_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"


void SystemClock_Config();
void Sys_DelayUs(int us);
void Sys_DelayMs(int ms);
void GlobalFunctions_Init(void);
void SystemClock_Config(void);
void init_all();
void init_sebesseg_mero_timer();

//#define UNUSED(a) (void)a;

void init_LED2();

extern TIM_HandleTypeDef Tim7Handle;

uint8_t vonalak_szama();
uint8_t vil_ledek_szama();
uint8_t csikok_szama(uint32_t);
void korforgalom_jelzes_felismeres();
void fal_felismeres();
void sebesseg_szabalyzas();
void jelzes_felismeres(uint8_t);
void kereszt_vonal_felismeres(uint8_t);
void konvoj_elhaladas_felismeres();
uint8_t fekez();

extern uint8_t new_cycle;

uint16_t capture_ertek;

extern float speed_of_drogon;

#define ONE_INC_IN_METER 0.00003f
#define L_SENSORS 182.0f
#define SENSORS_DIFF 0.8135f
#define ElSO_KORR_MM 2.9845f
#define HATSO_KORR_MM 3.721f

#define RADIAN_TO_DEGREE_CONV 57.2958f

typedef enum {
	START = 1,
	GYORSASAGI_SZAKASZ = 2,
	LASSITAS = 3,
	KANYAR = 4

} Robot_state;


/**************************/
/* Giroszkóp alapértékek */
#define X_ALAP 0
#define Y_ALAP 0
#define Z_ALAP 982
/**************************/


/***********************************/
/* Fekezes */
uint16_t fek_kozep_ido_milisec;
uint16_t fek_hatra_ido_milisec;
uint8_t fek_varjuk_meg_a_kozep_erteket;
uint8_t fek_varjuk_meg_a_hatra_erteket;
uint8_t fek_megvartuk_a_hatrat;
uint8_t fek_megvartuk_a_kozepet;
/***********************************/



/***********************************/
/* Fekezes utáni várakozás*/
uint16_t fek_varakozasi_ido;
uint8_t varjunk_egy_kicsit;
/***********************************/


#endif /* BSP_H_ */
