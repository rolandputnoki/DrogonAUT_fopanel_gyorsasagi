#include "bsp.h"
#include "szervo_pwm.h"
#include "motor_pwm.h"
#include "uart_communication.h"
#include "bsp.h"
#include "adc.h"
#include "dma.h"
#include "encoder.h"
#include "gyorsasagi_parameterek.h"

#include "sharp_hosszu.h"
#include "sharp_rovid.h"


/***********************************************************/
/****************Adat fogadás a szenzorpaneltõl************/
//SPI1
SPI_HandleTypeDef spi;
//SPI fogadási flag
uint8_t data_received = 1;

/***********************************************************/


/***********************************************************/
/***Minden BLuetooth kiiratás ezzel a flaggel védett********/
/* Állítsd nullába, hogy ne legyen Bluetooth küldés        */
uint8_t teszt_helyzet = 1;

/***********************************************************/


/***********************************************************/
/*** Kormány szabályzás paramétei********/
/*         */
uint16_t KP_kormany = 0;
uint16_t KD_kormany = 0;
uint8_t sebesseg_szabalyzas_on = 1;
/***********************************************************/


uint8_t start_kapu_fala_meg_volt = 0;


/***********************************************************/
/****************Pozíció értékek számolása a két panelre***/
float prev_pos = 0.0f;
float p_elso = 0.0f;
float p_hatso = 0.0f;
float p_atmenet = 0.0f;
float D = 0.0f;
int32_t p_prev_konv = 0;
float error = 0.0f;
float prev_error = 0.0f;
float setValue = 0.0f;
int16_t pd_value = 0;
/***********************************************************/



/***********************************************************/
/****************Sebesség szabályzoás paraméterei***/
/****************PI szabályzó***/
uint16_t KP_speed = 80;
uint16_t KI_speed = 5;
float integrator_ertek = 0;

float wanted_speed = 0.9f;
uint8_t sebesseg_tarto_counter = 0;

float speed_diff = 0;

uint8_t indulhat = 0;
uint8_t fekezni_kell = 0;
/***********************************************************/


/**********************************/
/*     Sharp szenzorok adata      */
uint16_t elulso_sharp_szenzor;
uint16_t jobb_oldali_sharp_szenzor;
uint16_t bal_oldali_sharp_szenzor;

uint32_t adc_eredmeny_jobb = 0;
uint32_t adc_eredmeny_bal = 0;
uint32_t adc_eredmeny_elso = 0;
/**********************************/



Robot_state state_of_robot = START;



/**********************************/
/*     Lassulás jelzõ után megtett távolság     */
int32_t lassitas_jelzo_kezdete = 0;
int32_t lassitas_jelzo_mostani = 0;
int32_t lassitas_jelzo_hossza = 0;
/**********************************/


uint8_t valtottunk_gyors_bol_lassuba = 0;
uint8_t valtottunk_lassubol_gyorsba = 0;

int main()
{

 	KP_kormany = KP_lassu;
	KD_kormany = KD_lassu;

	// HAL_Init, System_Clock_config és hardware inicializáció
    init_all();
    set_gyari_motor_compare_value(6200);
    HAL_Delay(2000);



	char buffer[10];
	HAL_ADC_Start_DMA(&hadc1, &adc_eredmeny_bal, (uint32_t)1);
	HAL_ADC_Start_DMA(&hadc2, &adc_eredmeny_elso, (uint32_t)1);
	HAL_ADC_Start_DMA(&hadc3, &adc_eredmeny_jobb, (uint32_t)1);



	while(1)
	{
		if(new_cycle)
		{
			elulso_sharp_szenzor = sharp_tomb_hosszu[adc_eredmeny_elso];
			jobb_oldali_sharp_szenzor = sharp_tomb_rovid[adc_eredmeny_jobb];
			bal_oldali_sharp_szenzor = sharp_tomb_rovid[adc_eredmeny_bal];

			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
			{

			} else {
				indulhat = 1;
			}
			ciklus();
		}

	}

}


//Az elsõ SPI adatfogadás hibás
//TODO: Gyere rá miért
uint8_t first_cycle = 1;

/*****************************************/
//Változók az spi adatfogadáshoz
//és pozíció számoláshoz mindkét szenzorsor esetén
/*****************************************/
uint16_t adcAdatok_hatso[32];
uint16_t adcAdatok_elso[32];

uint16_t adcAdatok_buffer_hatso[32];
uint16_t adcAdatok_buffer_elso[32];

int32_t adcAdatok_sulyozott_elso[32];
int32_t adcAdatok_sulyozott_hatso[32];

uint32_t szumma_adc_values_elso = 0;
int32_t szumma_sulyozott_elso = 0;

uint32_t szumma_adc_values_hatso = 0;
int32_t szumma_sulyozott_hatso = 0;
/*****************************************/


int32_t meres_kezdeti_encoder_ertek = 0;
int32_t meres_mostani_encoder_ertek = 0;
int32_t meres_megtett_tavolsag = 0;

int32_t gyors_meres_kezdeti_encoder_ertek = 0;
int32_t gyors_meres_mostani_encoder_ertek = 0;
int32_t gyors_meres_megtett_tavolsag = 0;
uint8_t most_kezdem_merni_a_gyorsitast = 0;


int32_t lassitas_meres_kezdeti_encoder_ertek = 0;
int32_t lassitas_meres_mostani_encoder_ertek = 0;
int32_t lassitas_meres_megtett_tavolsag = 0;
uint8_t most_kezdem_merni_a_lassitast = 0;


uint8_t lassitas_utani_parametert_valtottunk = 0;

char buf10[20];


uint8_t vilagito_ledek_szama = 0;

void ciklus(){

	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_12);
	szumma_adc_values_elso = 0;
	szumma_sulyozott_elso = 0;

	szumma_adc_values_hatso = 0;
	szumma_sulyozott_hatso = 0;


	// Adatfogadás kezdete
	data_received = 0;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	HAL_SPI_Receive_IT(&spi, adcAdatok_buffer_hatso, 64);
	while(!data_received);


	data_received = 0;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_SPI_Receive_IT(&spi, adcAdatok_buffer_elso, 64);
	while(!data_received);

	// Adatfogadás vége

	if(first_cycle){
		first_cycle = 0;

		meres_kezdeti_encoder_ertek = get_encoder_counter();
	} else {

		// Csak a világító ledekhez tartozó AD adatokat tartjuk meg
		for(uint8_t n = 0; n<32; n++){

			if(adcAdatok_buffer_hatso[n] <= 2000){

				adcAdatok_hatso[n] = 0;
			} else {
				adcAdatok_hatso[n] = adcAdatok_buffer_hatso[n];
			}

			if(adcAdatok_buffer_elso[n] <= 2000){

				adcAdatok_elso[n] = 0;
			} else {
				adcAdatok_elso[n] = adcAdatok_buffer_elso[n];
			}
		}

		vilagito_ledek_szama = vil_ledek_szama();

/****************************************************************/
		/* Itt kell még módosítani az adcAdatokon, ha tolatni akarunk  */
/****************************************************************/

		//arányos tényezõ számítása
		for(int i = 0; i < 32; i++)
		{
			szumma_adc_values_elso += adcAdatok_elso[i];
			adcAdatok_sulyozott_elso[i] = adcAdatok_elso[i] * sorszam[i];			//súlyozás
			szumma_sulyozott_elso += adcAdatok_sulyozott_elso[i];

			szumma_adc_values_hatso += adcAdatok_hatso[i];
			adcAdatok_sulyozott_hatso[i] = adcAdatok_hatso[i] * sorszam[i];			//súlyozás
			szumma_sulyozott_hatso += adcAdatok_sulyozott_hatso[i];

		}

		//Elsõ szenzorsor pozíció
		if(szumma_adc_values_elso != 0){
			p_elso = (float)szumma_sulyozott_elso/szumma_adc_values_elso;
		} else {
			p_elso = 0;
		}

		//Hátsó szenzorsor pozíció
		if(szumma_adc_values_hatso != 0){
			p_hatso = (float)szumma_sulyozott_hatso/szumma_adc_values_hatso;
		} else {
			p_hatso = 0;
		}

/****************************************************************/

		/*** Idáig tart a pozíció kiszámolása ***/
/****************************************************************/
//PD szabályzoó egy vonalszenzor alapján



		if(state_of_robot == KANYAR)
		{
			if(prev_pos > 12 || prev_pos < -13){
				p_atmenet = p_elso;
				if(p_atmenet < 0){
					p_atmenet *= -1;
				}
				if(p_atmenet < 3){
					p_elso = prev_pos;
				}
			}
		}

		//Differencia az elõzõ pozíció hibától
		D = p_elso-prev_pos;





/*
		if(!vil_ledek_szama()){
			p_elso = prev_pos;
		}
*/

/*
		if(state_of_robot == GYORSASAGI_SZAKASZ)
		{
			KP_kormany = KP_gyors;
			KD_kormany = KD_gyors;
		} else if(state_of_robot = LASSITAS)
		{
			if(meres_megtett_tavolsag >= 2800)
			{
				KP_kormany = KP_lassu;
				KD_kormany = KD_lassu;
			}
		}
*/

		pd_value = KP_kormany*p_elso + KD_kormany*D;
		prev_pos = p_elso;


//		szervo_value = DIGIT_SZ_KOZEP + (int16_t)pd_value;
		szervo_value = 33000 + (int16_t)pd_value;
		set_compare_value_digit_szervo(szervo_value);


		meres_mostani_encoder_ertek = get_encoder_counter();
		meres_megtett_tavolsag = (meres_kezdeti_encoder_ertek - meres_mostani_encoder_ertek)*ENCODER_VALUE_TO_MM;

		/*
		BT_UART_SendString("Megtett:  ");
		itoa(meres_megtett_tavolsag, buf10, 10);
		BT_UART_SendString(buf10);
		BT_UART_SendString("\r\n");
*/
		switch (state_of_robot) {
			case GYORSASAGI_SZAKASZ:

				if(most_kezdem_merni_a_gyorsitast)
				{

					most_kezdem_merni_a_gyorsitast = 0;
				} else {
					gyors_meres_mostani_encoder_ertek = get_encoder_counter();
					gyors_meres_megtett_tavolsag = (gyors_meres_kezdeti_encoder_ertek - gyors_meres_mostani_encoder_ertek)*ENCODER_VALUE_TO_MM;

					if(gyors_meres_megtett_tavolsag >= 400)
					{
						wanted_speed = HIGH_SPEED;

						KP_kormany = KP_gyors;
						KD_kormany = KD_gyors;


					}
				}


				BT_UART_SendString("Gyors resz:  ");
				itoa(gyors_meres_megtett_tavolsag, buf10, 10);
				BT_UART_SendString(buf10);
				BT_UART_SendString("\r\n");

				break;

			case LASSITAS:
				if(most_kezdem_merni_a_lassitast)
				{
	//				lassitas_meres_kezdeti_encoder_ertek = get_encoder_counter();
					most_kezdem_merni_a_lassitast = 0;
				} else {
					lassitas_meres_mostani_encoder_ertek = get_encoder_counter();
					lassitas_meres_megtett_tavolsag = (lassitas_meres_kezdeti_encoder_ertek - lassitas_meres_mostani_encoder_ertek)*ENCODER_VALUE_TO_MM;

					itoa(lassitas_meres_megtett_tavolsag, buf10, 10);
					BT_UART_SendString(buf10);
					BT_UART_SendString("\r\n");


					if(lassitas_meres_megtett_tavolsag >= 2500)
					{
						BT_UART_SendString("Megtett\r\n");
						fekezni_kell = 1;

					}

					if(fekezni_kell)
					{
						sebesseg_szabalyzas_on = 0;
						if(fekez())
						{
							BT_UART_SendString("F VEGE\r\n");


							fekezni_kell = 0;
							fek_hatra_ido_milisec = 0;
							fek_kozep_ido_milisec = 0;
							fek_megvartuk_a_hatrat = 0;
							fek_megvartuk_a_kozepet = 0;
							fek_varjuk_meg_a_hatra_erteket = 0;
							fek_varjuk_meg_a_kozep_erteket = 0;
							most_kezdem_merni_a_lassitast = 1;
							state_of_robot = KANYAR;
						}
					}
				}

				break;


			case KANYAR:

				KP_kormany = KP_lassu;
				KD_kormany = KD_lassu;
				sebesseg_szabalyzas_on = 1;
				wanted_speed = LOW_SPEED;
				break;


			case START:


			default:
				break;
		}

		if(teszt_helyzet)
		{
			switch (state_of_robot) {
				case LASSITAS:
					BT_UART_SendString("LASSU\r\n");
					break;

				case GYORSASAGI_SZAKASZ:
					BT_UART_SendString("GYORS\r\n");
					break;

				case KANYAR:
					BT_UART_SendString("KANYAR\r\n");
					break;

				case START:
					BT_UART_SendString("START\r\n");
					break;

				default:
					break;
			}
		}


		if(indulhat)
		{
			if(sebesseg_szabalyzas_on)
			{
				sebesseg_szabalyzas();
			}

		}

		jelzes_felismeres(vonalak_szama());

/*
		itoa(vonalak_szama(), buf10, 10);
		BT_UART_SendString(buf10);
		BT_UART_SendString("\r\n");
*/
/*******************************************/
//Ciklus újra indítása csak a timer lejárta után történhet meg. Ezért kell nullázni,
//hogy ha a ciklus véget is ért, a main-ben ne hívódjon meg újra, míg a timer owerflowja
//be nem billenti a flaget
		new_cycle = 0;
/*******************************************/
	}
}



uint8_t vonalak_szama(){
	uint8_t csucs_kezdete = 0;
	uint8_t cnt = 0;
	uint8_t m;
	for(m = 0; m < 32; m++){
		if(!csucs_kezdete){
			if(adcAdatok_elso[m] != 0){
				csucs_kezdete = 1;

			}
		} else {
			if(adcAdatok_elso[m] == 0){
				csucs_kezdete = 0;
				cnt++;
			}
		}
	}
	return cnt;
}

uint8_t vil_ledek_szama()
{
	uint8_t cnt = 0;
	uint8_t m = 0;
	for(m = 0; m < 32; m++)
	{
		if(adcAdatok_elso[m] != 0){
			cnt++;
		}
	}
	return cnt;
}


/***************************/
//Jelzes felismerés segédváltozók

//3 vonalhoz -----> Lassításhoz és gyorsításhoz
uint8_t hanyszor_volt_3_vonal = 0;
uint8_t harmas_vonal_van = 0;
uint8_t elobb_3_vonal_volt = 0;
int32_t harmas_vonal_kezdete_encoder_ertek = 0;
int32_t harmas_vonal_vege_encoder_ertek = 0;
int32_t harmas_vonal_mostani_encoder_ertek = 0;
int32_t harmas_vonal_hossza = 0;

uint8_t hanyszor_volt_1_vonal = 0;
uint8_t elobb_1_vonal_volt = 0;
uint8_t egyes_vonal_van = 0;

/***************************/

/*
void jelzes_felismeres(uint8_t vonal_szam){

	if(!harmas_vonal_van)
	{
		if(vonal_szam == 3)
		{
			if(elobb_3_vonal_volt){
				hanyszor_volt_3_vonal++;
			}

			if(hanyszor_volt_3_vonal >= 2){
				harmas_vonal_kezdete_encoder_ertek = get_encoder_counter();
				harmas_vonal_van = 1;
			}

			if(!elobb_3_vonal_volt){
				elobb_3_vonal_volt = 1;
			}
		}
	} else
	{
		harmas_vonal_vege_encoder_ertek = get_encoder_counter();
		harmas_vonal_hossza = (harmas_vonal_kezdete_encoder_ertek - harmas_vonal_vege_encoder_ertek)*ENCODER_VALUE_TO_MM;

		if(harmas_vonal_hossza > 110)
		{
			state_of_robot = LASSITAS;
			lassitas_jelzo_kezdete = get_encoder_counter();
		}
		if(vonal_szam == 1)
		{
			if(elobb_1_vonal_volt){
				hanyszor_volt_1_vonal++;
			}
			if(hanyszor_volt_1_vonal >= 2){
				harmas_vonal_van = 0;
				elobb_3_vonal_volt = 0;
				hanyszor_volt_3_vonal = 0;
				harmas_vonal_hossza = 0;

				elobb_1_vonal_volt = 0;
				hanyszor_volt_1_vonal = 0;


				if(harmas_vonal_hossza < 100){
					state_of_robot = GYORSASAGI_SZAKASZ;
				}
			}
			if(!elobb_1_vonal_volt){
				elobb_1_vonal_volt = 1;
			}
		}
	}
}
*/

void jelzes_felismeres(uint8_t vonal_szam){




	if(harmas_vonal_van)
	{
		harmas_vonal_vege_encoder_ertek = get_encoder_counter();
		harmas_vonal_hossza = (harmas_vonal_kezdete_encoder_ertek - harmas_vonal_vege_encoder_ertek)*ENCODER_VALUE_TO_MM;

		BT_UART_SendString("3V: ");
		itoa(harmas_vonal_hossza, buf10, 10);
		BT_UART_SendString(buf10);
		BT_UART_SendString("\r\n");

		if(harmas_vonal_hossza > 110)
		{

			if(!valtottunk_gyors_bol_lassuba)
			{
				BT_UART_SendString("VÁLT LASSÚ\r\n");
				state_of_robot = LASSITAS;
				lassitas_meres_kezdeti_encoder_ertek = get_encoder_counter();
				valtottunk_lassubol_gyorsba = 0;
				valtottunk_gyors_bol_lassuba = 1;
			}

		}
		if(vonal_szam == 1)
		{
			harmas_vonal_van = 0;
			elobb_3_vonal_volt = 0;
			hanyszor_volt_3_vonal = 0;

			if(harmas_vonal_hossza < 100){

				if(!valtottunk_lassubol_gyorsba)
				{
					state_of_robot = GYORSASAGI_SZAKASZ;
					gyors_meres_kezdeti_encoder_ertek = get_encoder_counter();
					valtottunk_lassubol_gyorsba = 1;
					valtottunk_gyors_bol_lassuba = 0;
				}

			}
			harmas_vonal_hossza = 0;
		}
	} else
	{
		if(vonal_szam == 3)
		{
			if(elobb_3_vonal_volt){
				hanyszor_volt_3_vonal++;
			}

			if(hanyszor_volt_3_vonal >= 2){
				harmas_vonal_kezdete_encoder_ertek = get_encoder_counter();
				harmas_vonal_van = 1;
				hanyszor_volt_3_vonal = 0;
				elobb_3_vonal_volt = 0;
			}

			if(!harmas_vonal_van){
				elobb_3_vonal_volt = 1;
			}

		}
	}

}


void send_adc_values_over_bt(void){
	uint8_t i = 0;
	char buff[5];

	BT_UART_SendString("Hatso_szenzor:\r\n");
	for(i = 0; i<32; i++){
		buff[0] = 0;
		buff[1] = 0;
		buff[2] = 0;
		buff[3] = 0;
		buff[4] = 0;
		itoa(adcAdatok_buffer_hatso[i], buff, 10);
		BT_UART_SendString(buff);
		BT_UART_SendString(" ");
	}
	BT_UART_SendString("\r\n");
	BT_UART_SendString("Elso_szenzor:\r\n");
	for(i = 0; i<32; i++){
		buff[0] = 0;
		buff[1] = 0;
		buff[2] = 0;
		buff[3] = 0;
		buff[4] = 0;
		itoa(adcAdatok_buffer_elso[i], buff, 10);
		BT_UART_SendString(buff);
		BT_UART_SendString(" ");
	}
	BT_UART_SendString("\r\n");
}


void sebesseg_szabalyzas(){
	if(sebesseg_tarto_counter > 5){
		speed_diff = wanted_speed - speed_of_drogon;

		integrator_ertek += speed_diff;
		motor_value = 6480 + (int)(speed_diff*KP_speed) + (int)(KI_speed*integrator_ertek);

		if(motor_value > GYARI_MOTOR_COUNTER_MAX){
			motor_value = GYARI_MOTOR_COUNTER_MAX;
		} else if(motor_value < GYARI_MOTOR_COUNTER_KOZEP){
			motor_value = GYARI_MOTOR_COUNTER_KOZEP;
		}
		set_gyari_motor_compare_value(motor_value);

	sebesseg_tarto_counter = 0;
	} else {
	sebesseg_tarto_counter++;
	}
}

/*******************************************************/
/* Fékezés segédváltozói  */
uint8_t lefekeztunk = 0;
/*******************************************************/


uint8_t fekez()
{

	if(!fek_megvartuk_a_kozepet)
	{

		if(!fek_varjuk_meg_a_kozep_erteket){
			if(!fek_varjuk_meg_a_hatra_erteket){
				fek_varjuk_meg_a_hatra_erteket = 1;
				set_gyari_motor_compare_value(5800);
			} else {

				if(fek_hatra_ido_milisec >= 50){
					fek_varjuk_meg_a_kozep_erteket = 1;
					set_gyari_motor_compare_value(6200);
					fek_megvartuk_a_hatrat = 1;
				}
			}
		} else {
			if(fek_kozep_ido_milisec >= 50){
				fek_megvartuk_a_kozepet = 1;
				set_gyari_motor_compare_value(4800);
			}
		}
	} else {


		if(speed_of_drogon <= 0.0f)
		{
			set_gyari_motor_compare_value(6200);
			lefekeztunk = 1;
		} else
		{
			set_gyari_motor_compare_value(4800);
		}

	}


	return lefekeztunk;
}


