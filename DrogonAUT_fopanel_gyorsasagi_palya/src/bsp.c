/*
 * bsp.c
 *
 *  Created on: 2017. okt. 30.
 *      Author: Roland
 */

#include <szervo_pwm.h>
#include "bsp.h"
#include "encoder.h"



/***********************************/
/* Fekezes */
extern uint16_t fek_kozep_ido_milisec = 0;
extern uint16_t fek_hatra_ido_milisec = 0;
extern uint8_t fek_varjuk_meg_a_kozep_erteket = 0;
extern uint8_t fek_varjuk_meg_a_hatra_erteket = 0;
extern uint8_t fek_megvartuk_a_hatrat = 0;
extern uint8_t fek_megvartuk_a_kozepet = 0;
/***********************************/



/***********************************/
/*Lassit */
extern uint16_t lassit_kozep_ido_milisec = 0;
extern uint16_t lassit_hatra_ido_milisec = 0;
extern uint8_t lassit_varjuk_meg_a_kozep_erteket = 0;
extern uint8_t lassit_varjuk_meg_a_hatra_erteket = 0;
extern uint8_t lassit_megvartuk_a_hatrat = 0;
extern uint8_t lassit_megvartuk_a_kozepet = 0;
/***********************************/


/***********************************/
/* Fekezes utáni várakozás*/
extern uint16_t fek_varakozasi_ido = 0;
extern uint8_t varjunk_egy_kicsit = 0;
/***********************************/


/**************************************************************/
/* Konvoj elhaladás felismerés segédváltozók  */
/* Konvoj elhaladásának méréséhez */
extern uint16_t egy_auto_elhaladasanak_ideje = 0;
extern uint16_t egy_res_ideje = 0;
extern uint8_t most_rest_merunk = 0;
extern uint8_t most_autot_merunk = 0;

/* Egyszerûbb változathoz*/

extern uint8_t autot_erzekeltem = 0;
extern uint16_t varakozasi_ido = 0;
/**************************************************************/


int16_t x_correction, y_correction, z_correction;

volatile int sys_delay = 0;
uint8_t new_cycle = 0;


/** Globï¿½lis funkciï¿½k inicializï¿½lï¿½sa. */


TIM_HandleTypeDef Tim7Handle, Tim9Handle;


void init_all(){
	HAL_Init(); 			//init HAL library
	SystemClock_Config(); 	//set system clock



	//Start kapuhoz
//	Radio_UART_Init();

	//Szervo indítás
	Init_AF_szervo();
	Init_Digit_Szervo_PWM();

	//Gyári motor indítás
	InitAF_gyari_motor();
	Init_gyari_motor_PWM();

	//Motor input capture
//	set_gy_rv_af_motor();
//	Init_input_capture_motor();

	//Gyorsulásmérõ
	I2C_Init(0);
	LSM6DS3_Init2();

	SPI1_init();

	/* Bluetooth bekapcsolása */
	/* AutoReconnect engedélyezve van, így rögtön csatlakozik */
	BT_init_pins();
	BT_UART_Init();

	//Sharp szenzorok méréséhez az ADC csatornák inicializációja
	ADC_Init();
	ConfigureDMA();
	//
	init_pin_to_analyser();

	//A mérés ciklusidejét határozza meg
	init_cuklus_timer();


	//Enkóder
	init_mag_inc_dec_pin();
	encoder_init();

	init_sebesseg_mero_timer();

	//Infra vevõ
	init_infra_rev_pin();
//	init_infra_timer();

	Set_LMS6DS3_correction_values();


	init_dip_kapcsolo();
}

void init_cuklus_timer(void)
{

	//Tim7 84MHz
	__TIM7_CLK_ENABLE();

	Tim7Handle.Instance = TIM7;
	Tim7Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim7Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim7Handle.Init.Prescaler = 6;
	Tim7Handle.Init.Period = 52499;
	Tim7Handle.State = HAL_TIM_STATE_RESET;
	HAL_TIM_Base_Init(&Tim7Handle);
	HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_TIM_Base_Start_IT(&Tim7Handle);
}



void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&Tim7Handle);
}


void TIM1_BRK_TIM9_IRQHandler(void){
	HAL_TIM_IRQHandler(&Tim9Handle);
}



/** us szï¿½mï¿½ mikroszekundum vï¿½rakozï¿½s (blokkolva). */
void Sys_DelayUs(int us)
{
	sys_delay = us;
	while(sys_delay);
}

/** ms szï¿½mï¿½ milliszekundum vï¿½rakozï¿½s (blokkolva). */
void Sys_DelayMs(int ms)
{
	sys_delay = ms*1000;
	while(sys_delay);
}

/* ----------------- Megszakï¿½tï¿½skezelï¿½ ï¿½s callback fï¿½ggvï¿½nyek ----------------- */


uint32_t prev_inc_value = 0;
uint32_t now_inc_value = 0;
int32_t inc_difference;

float speed_of_drogon = 0;

char s_buff[30];

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *handle)
{
	UNUSED(handle);

	if (sys_delay>0)
	{
		sys_delay--;
	}

	if (handle->Instance == TIM7)
	{
			new_cycle = 1;
	}

	if (handle->Instance == TIM9)
	{
		if(fek_varjuk_meg_a_kozep_erteket){
			fek_kozep_ido_milisec++;
			if(fek_megvartuk_a_kozepet){
				fek_kozep_ido_milisec = 0;
				fek_varjuk_meg_a_kozep_erteket = 0;
			}
		}

		if(fek_varjuk_meg_a_hatra_erteket){
			fek_hatra_ido_milisec++;
			if(fek_megvartuk_a_hatrat){
				fek_hatra_ido_milisec = 0;
				fek_varjuk_meg_a_hatra_erteket = 0;
			}
		}


		if(lassit_varjuk_meg_a_kozep_erteket){
			lassit_kozep_ido_milisec++;
			if(lassit_megvartuk_a_kozepet){
				lassit_kozep_ido_milisec = 0;
				lassit_varjuk_meg_a_kozep_erteket = 0;
			}
		}

		if(lassit_varjuk_meg_a_hatra_erteket){
			lassit_hatra_ido_milisec++;
			if(lassit_megvartuk_a_hatrat){
				lassit_hatra_ido_milisec = 0;
				lassit_varjuk_meg_a_hatra_erteket = 0;
			}
		}


		if(varjunk_egy_kicsit)
		{
			fek_varakozasi_ido++;
		}



		now_inc_value = get_encoder_counter();
		inc_difference = -1*(now_inc_value - prev_inc_value);
		speed_of_drogon = ENCODER_VALUE_TO_MM*(float)inc_difference;
		prev_inc_value = now_inc_value;

	}
}


/*speed_of_drogon

void TIM4_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&Tim4Handle);
}

*/

void init_LED2(void)
{
		__GPIOA_CLK_ENABLE();
		GPIO_InitTypeDef GPIO_InitStructure;
		// Configure pin in output push/pull mode
		GPIO_InitStructure.Pin = GPIO_PIN_5;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
}

void init_pin_to_analyser(){
	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();


	GPIO_InitTypeDef GPIO_InitStructure;
	// Configure pin in output push/pull mode
	GPIO_InitStructure.Pin = GPIO_PIN_12;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);


	GPIO_InitStructure.Pin = GPIO_PIN_12;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

}

void init_user_button(void)
{
	__GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_13;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void init_dip_kapcsolo(void)
{
	__GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
}



/* System Clock Configuration */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void init_mag_inc_dec_pin(){
	__GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_9;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void init_sebesseg_mero_timer()
{
	//Tim9 168 MHz
	__TIM9_CLK_ENABLE();

	Tim9Handle.Instance = TIM9;
	Tim9Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	Tim9Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim9Handle.Init.Prescaler = 167;
	Tim9Handle.Init.Period = 999;
	Tim9Handle.State = HAL_TIM_STATE_RESET;
	HAL_TIM_Base_Init(&Tim9Handle);
	HAL_NVIC_SetPriority(TIM1_BRK_TIM9_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
	HAL_TIM_Base_Start_IT(&Tim9Handle);
}

void Set_LMS6DS3_correction_values()
{
	int32_t x,y,z;
	if(LMS6DS3_Read_Axes(&x,&y,&z)){
		BT_UART_SendString("Baj van!");
	}
	x_correction = x - X_ALAP;
	y_correction = y - Y_ALAP;
	z_correction = z - Z_ALAP;
}

void LMS6DS3_Read_Axes_with_correction(int32_t *x_axis, int32_t *y_axis, int32_t *z_axis){
	if(LMS6DS3_Read_Axes(x_axis, y_axis, z_axis))
	{
		BT_UART_SendString("Baj van!");
	}
	*x_axis -= x_correction;
	*y_axis -= y_correction;
	*z_axis -= z_correction;
}

