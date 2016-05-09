	/**
  **************************************************************************************
  * @file    manip.c
  * @author  Simen W. Tofteberg
  * @version V1.0
  * @date    09-February-2016
  * @brief   Metoder til manipulator
  **************************************************************************************
  */

/* -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
 * Please keep lines of comments and code less than 90 characters long.
 * -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
 */

/* Include------------------------------------------------------------------------------*/

  /* Include .h files from other modules here.*/
#include "manip.h" // h-file belonging to this c-file
#include "stm32f10x.h" // h-file specific to the microcontroller. Allways include this.
#include "stm32f10x_can.h"  // h-file that declares functions from another c-file.
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "semihosting.h"
#include "POWR_canbus.h"
#include "POWR_rov2016_Interface.h"
#include "POWR_declare_global_vars.h"
#include <stdio.h>
#include <stdlib.h>

/* Global variables --------------------------------------------------------------------*/
/* If needed, include header file for extern declarations of global variables here.
 * Try to avoid global variables. Using getters and setters is usually a better
 * alternative.
 */
#include "manip.h"


/* Private Function Prototypes ----------------------------------------------------------*/

  /* Put prototypes for static functions here.
   * Private/static functions can only be called from within this c-file. Use static
   * for all functions that does not need to be used by an outside program.
   */
   static void GPIO_Oppstart_Enkoder(void);
   static void Vent(int tid);
   static uint16_t Vinkel_Grader(uint16_t pos);
   static void Hent_SettPunkt(void);



/* Private variables -------------------------------------------------------------------*/
  /* Put static/private variables here. These variables are only accessible from this
   * .c-file.
   */
   static int16_t* kontroller_manip;
   static uint8_t motor_strom_teller = 9;
   static uint8_t totalt_antall_motorer = 12;

   static int t = 1;
   static uint16_t buffer_r = 0x0; // Note: remember to initialize variables. (=0;)
   static uint16_t buffer_s = 0x0;
   static uint16_t vinkel_r = 0;
   static uint16_t vinkel_s = 0;

   static uint8_t byte_2 = 2;
   static uint8_t byte_3 = 3;
   static uint8_t byte_4 = 4;
   static uint8_t byte_5 = 5;
   static uint8_t CAN_buffer = 0;
   static uint16_t CAN_buffer16 = 0;

   static int16_t sp_r = 0;
   static int hold_r = 0;
   static int16_t sp_s = 0;
   static int hold_s = 0;
   static int flagg_forste = 1;

   static int16_t e_r = 0;
   static int16_t e_s = 0;
   static int16_t ge_r = 0;
   static int16_t ge_s = 0;
   static float padrag_r = 0.0f;
   static float padrag_s = 0.0f;
   static uint16_t T_s = 0.1;
   static uint16_t Kp_r = 0.1;
   static uint16_t Kp_s = 0.1;
   static uint16_t Ti_r = 1;
   static uint16_t Ti_s = 1;
   static uint16_t Td_r = 1;
   static uint16_t Td_s = 1;
   static int16_t uig_r = 0;
   static int16_t uig_s = 0;

   static uint8_t strom_grip = 0;
   static uint8_t strom_stamp = 0;
   static uint8_t strom_rull = 0;
   static uint8_t strom_tele = 0;

/*
   static uint16_t CCR3_Val = 1500;
   static uint16_t CCR4_Val = 1500;
   static PrescalerValue = 24;
   static uint16_t periode = 20000;
*/
   static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   //static TIM_OCInitTypeDef  TIM_OCInitStructure;

   static float padrag_t = 0.0f;
   static uint8_t pad_up = 0;
   static uint8_t pad_down = 0;

   static float padrag_g = 0.0f;
   static int16_t avtrekker = 0;
   static uint8_t flagg_inn = 0;
   static uint8_t flagg_ut = 0;
///* Function definitions ----------------------------------------------------------------*/
//
///* Define all functions here. Use the static keyword for private functions and the
// * extern keyword for public functions. Document the function by writing what it does
// * over the function definition.
///**
// * @brief  Explain what this function does here.
// * @param  uint8_t parameter: bla bla bla... can be a value of: ...
// * @retval Returns the number of...
// */
   static void Hent_SettPunkt(void){
//	   A = 0; B = 0; X = 0; Y = 0;
//	   CAN_buffer = CAN_getByteFromMessage(CAN_RX_FILTER_2, byte_4);
//	   if (CAN_buffer & 0x1)A = 1;
//	   if ((CAN_buffer>>1) & 0x1)B = 1;
//	   if ((CAN_buffer>>2) & 0x1)X = 1;
//	   if ((CAN_buffer>>3) & 0x1)Y = 1;

	   /* Setter settpunkt for rull */
	   if (kontroller_manip[4] > 24) { // Hoyre styrestake er positiv mot hoyre
		sp_r += (1 + kontroller_manip[4]/100); // Flytter settpunkt i positiv retning
		hold_r = 0;
	   }else if (kontroller_manip[4] < -24) { // Hoyre styrespake er negativ mot venstre
		sp_r -= (1 + abs(kontroller_manip[4]/100)); // Flytter settpunkt i negativ retning
		hold_r = 0;
	   }else if ((-24 < kontroller_manip[4]) && (kontroller_manip[4] < 24) && (hold_r = 0)) { // Hysterese for ikke endret settpunkt
		sp_r = vinkel_r; // Onsker ikke aa endre naavearende posisjon
		hold_r = 1; // Holder verdien
	   }

	   /* Setter settpunkt for stamp */
	   if (kontroller_manip[2] > 24) { // Venstre styrestake er positiv mot hoyre
	   		sp_s += (1 + kontroller_manip[2]/100); // Flytter settpunkt i positiv retning
	   		hold_s = 0;
	   }else if (kontroller_manip[2] < -24) { // Venstre styrestake er negativ mot venstre
	   		sp_s -= (1 + abs(kontroller_manip[2]/100)); // Flytter settpunkt i negativ retning
	   		hold_s = 0;
	   }else if ((-24 < kontroller_manip[2]) && (kontroller_manip[2] < 24) && (hold_s = 0)) { // Hysterese for ikke endret settpunkt
	   	   		sp_s = vinkel_s; // Onsker ikke aa endre naavearende posi
	   	   		hold_s = 1;
	   }

	   if (sp_s < 0)sp_s += 1024;
	   if (sp_s > 1024)sp_s -= 1024;
	   if (sp_r < 0)sp_r += 1024;
	   if (sp_r > 1024)sp_r -= 1024;
   }
   static void GPIO_Oppstart_Enkoder(void){
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		// GPIO setup for SPI
		//****************************************************************************
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // PA13 CLK, PA14 MISO
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14; // PA13 CLK, PA14 MISO
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIOB->ODR = GPIOB->ODR | GPIO_Pin_13; // Setter klokkesignalet høyt fra start

		//****************************************************************************
   }
//
//   static void Vent(int tid){
//   	volatile int tim = 0;
//   	while (tim < tid)tim++;
//  }
//   static uint16_t Vinkel_Grader(uint16_t pos){
//   	uint16_t v = (uint16_t) (pos/1024)*360;
//   	return v;
//   }
///**
// * @brief  Explain what this function does here.
// * @param  None
// * @retval None
// */
extern void Manip_Oppstart(void){
    GPIO_Oppstart_Enkoder();
}
extern void Les_Enkodere(void){
		GPIOB->ODR &= ~GPIO_Pin_13; //1 sett klokke lav
		Vent(t/2); //2 vent en halv periode
		int i;
		buffer_r = 0;
		buffer_s = 0;
		for(i = 0; i < 10; i++){
			GPIOB->ODR |= GPIO_Pin_13; //3 sett klokke høy
			Vent(t/2); //4 vent en halv periode
			buffer_s = buffer_s << 1; //5 flytt bit ett hakk
			buffer_r = buffer_r << 1;
			GPIOB->ODR &= ~GPIO_Pin_13; //6 sett klokke lav
			if(GPIOB->IDR & GPIO_Pin_14){//7 Les innganger
				buffer_s += 0x1;
			}
			if(GPIOB->IDR & GPIO_Pin_15){
				buffer_r += 0x1;
			}
			Vent(t/2); //8 vent en halv periode
		}
		GPIOB->ODR |= GPIO_Pin_13; //9 sett klokke høy
		vinkel_s = buffer_s;
		vinkel_r = buffer_r;

}
extern void Manip_Reguler(void){
  /* Henter settpunkt */
	Hent_SettPunkt();
	if (flagg_forste > 0){
		sp_r = vinkel_r;
		sp_s = vinkel_s;
		flagg_forste = 0;
	}

  /* Avviksutregning */
	e_r = sp_r - vinkel_r;
	e_s = sp_s - vinkel_s;
  if (e_r > 1000)e_r -= 1024;
  if (e_r < -1000)e_r += 1024;
  if (e_s > 500)e_s -= 1024/2 ;
  if (e_s < -500)e_s += 1024/2;

  /* Regulering rull */
  if (strom_rull < 25) {
		padrag_r = (float) Kp_r*e_r + uig_r + (Kp_r/Ti_r)*((e_r + ge_r)/2)*T_s + Kp_r*Td_r*(e_r - ge_r)/T_s;
	}else {
		padrag_r = 0.0f;
	}
  uig_r +=  (Kp_r/Ti_r)*((e_r + ge_r)/2)*T_s;
  ge_r = e_r;

  /* Regulering stamp */
  if (strom_stamp < 25) {
		padrag_s = (float) Kp_s*e_s + uig_s + (Kp_s/Ti_s)*((e_r + ge_r)/2)*T_s + Kp_s*Td_s*(e_s - ge_s)/T_s;
	}else {
		padrag_s = 0.0f;
	}
  uig_s += (Kp_s/Ti_s)*((e_s + ge_s)/2)*T_s;
  ge_r = e_r;

  /* Sender pådrag */
	VESC_setDutyCycle(ESC_ID_10, padrag_s*100.0f/(0.0171625f));
	VESC_setDutyCycle(ESC_ID_11, padrag_r*100.0f/(0.0171625f));
}
extern void  Manip_Teleskop(void){
//	pad_down = 0; pad_up = 0;
//	CAN_buffer = CAN_getByteFromMessage(CAN_RX_FILTER_2, byte_5);
//	if ((CAN_buffer>>5) & 0x1)pad_up = 1;
//	if ((CAN_buffer>>6) & 0x1)pad_down = 1;

	if ((kontroller_manip[11] > 0) && (strom_tele < 25)){
		padrag_t = 0.84f;
	}else if ((kontroller_manip[11] < 0) && (strom_tele < 25)) {
		padrag_t = -0.84f;
	}else {
		padrag_t = 0.0f;
	}
	VESC_setDutyCycle(ESC_ID_9, padrag_t);
}
//extern void Manip_Foresporsel_Strom(void){
//	Interface_VESC_requestData(motor_strom_teller, CAN_PACKET_GET_CURRENT);
//	Interface_VESC_requestData(motor_strom_teller + 1, CAN_PACKET_GET_CURRENT);
//	Interface_VESC_requestData(motor_strom_teller + 2, CAN_PACKET_GET_CURRENT);
//	Interface_VESC_requestData(motor_strom_teller + 3, CAN_PACKET_GET_CURRENT);
//	motor_strom_teller = 9;
//}
extern void Manip_Hent_Strom(void){
	/* Read messages from CAN receive buffer */
		strom_grip = CAN_getByteFromMessage(fmi_manip_motor_curr_9, 0);
		strom_stamp = CAN_getByteFromMessage(fmi_manip_motor_curr_10, 0);
		strom_rull = CAN_getByteFromMessage(fmi_manip_motor_curr_11, 0);
		strom_tele = CAN_getByteFromMessage(fmi_manip_motor_curr_12, 0);
}
extern void Manip_Kontroller_Oppdater(void){
	kontroller_manip = Interface_readController();
}

extern void  Manip_Grip_F(void){
	//Manip_Foresporsel_Strom();
	/* Bytte av kjøreretning siden stopp? */
	if ((flagg_ut > 0) && (kontroller_manip[7] > 0)) {
		flagg_ut = 0;
	}else if ((flagg_inn > 0) && (kontroller_manip[7] < 0)) {
		flagg_inn = 0;
	}
	//Manip_Hent_Strom();

	/* Gripepaadrag */
	if ((strom_grip < 25) && (kontroller_manip[7] > 0) && (flagg_ut < 1)){
		padrag_g = 0.50f;
	}else if ((strom_grip < 25) && (kontroller_manip[7] < 0) && (flagg_inn < 1)){
		padrag_g = -0.50f;
	}else {
		padrag_g = 0.0f;
		if (kontroller_manip[7] > 0) { //Flagger kjoreretningen ved stopp
			flagg_inn = 1;
		}else if (kontroller_manip[7] < 0){
			flagg_ut = 1;
		}}

	/* Sender motorpaadrag */
	VESC_setDutyCycle(ESC_ID_9, padrag_g);
}

//extern void  Manip_Grip(uint8_t strom){
//	CAN_buffer16 = 0; //Gjor klar buffer til overforing
//	CAN_buffer16 = CAN_getByteFromMessage(CAN_RX_FILTER_2, byte_3); //Mottar forste byte
//	CAN_buffer16 = CAN_buffer16<<8; //Flytter byte for aa gjore plass til neste
//	CAN_buffer16 += CAN_getByteFromMessage(CAN_RX_FILTER_2, byte_2); //Mottar andre byte
//	avtrekker = (int16_t) CAN_buffer16; //Oppdaterer avtrekker med ny verdi
//	avtrekker -= 1000;
//
//	if ((flagg_ut > 0) && (avtrekker > 0)) { //Sjekker om det er bytte av kjoreretningen siden forrige stopp
//		flagg_ut = 0;
//	}else if ((flagg_inn > 0) && (avtrekker < 0)) {
//		flagg_inn = 0;
//	}
//	if ((strom < 25) && (flagg_inn < 1) && (flagg_ut < 1)){ //Dersom det ikke er grunn for aa stanse, kjor
//		padragG = (uint8_t) avtrekker/(1000/122);
//	}else {
//		padragG = 123; //Grunn til aa stanse PWM = 0% ved padrag paa 123
//		if (avtrekker > 0) { //Flagger kjoreretningen ved stopp
//			flagg_inn = 1;
//		}else if (avtrekker < 0){
//			flagg_ut = 1;
//	}	}
//	VESC_setDutyCycle(ESC_ID_12, padragG); //Sender motorpaadrag
//}
