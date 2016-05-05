//	/**
//  **************************************************************************************
//  * @file    manip.c
//  * @author  Simen W. Tofteberg
//  * @version V1.0
//  * @date    09-February-2016
//  * @brief   Metoder til manipulator
//  **************************************************************************************
//  */
//
///* -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
// * Please keep lines of comments and code less than 90 characters long.
// * -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
// */
//
///* Include------------------------------------------------------------------------------*/
//
//  /* Include .h files from other modules here.*/
//#include "manip.h" // h-file belonging to this c-file
//#include "stm32f10x.h" // h-file specific to the microcontroller. Allways include this.
//#include "stm32f10x_can.h"  // h-file that declares functions from another c-file.
//#include "stm32f10x_gpio.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_tim.h"
//#include "semihosting.h"
//#include "POWR_canbus.h"
//#include "POWR_rov2016_Interface.h"
//#include "POWR_declare_global_vars.h"
//#include <stdio.h>
//#include <stdlib.h>
//
///* Global variables --------------------------------------------------------------------*/
///* If needed, include header file for extern declarations of global variables here.
// * Try to avoid global variables. Using getters and setters is usually a better
// * alternative.
// */
//#include "manip.h"
//
//
///* Private Function Prototypes ----------------------------------------------------------*/
//
//  /* Put prototypes for static functions here.
//   * Private/static functions can only be called from within this c-file. Use static
//   * for all functions that does not need to be used by an outside program.
//   */
//   static void GPIO_Oppstart_Enkoder(void);
//   static void Vent(int tid);
//   static uint16_t Vinkel_Grader(uint16_t pos);
//   static void Hent_SettPunkt(void);
//
//
//
///* Private variables -------------------------------------------------------------------*/
//  /* Put static/private variables here. These variables are only accessible from this
//   * .c-file.
//   */
//   static int16_t* controller_manip;
//   static uint8_t motor_strom_teller = 9;
//   static uint8_t totalt_antall_motorer = 12;
//
//   static int t = 1;
//   static uint16_t bufferD = 0x0; // Note: remember to initialize variables. (=0;)
//   static uint16_t bufferS = 0x0;
//   static uint16_t vinkelD = 0;
//   static uint16_t vinkelS = 0;
//
//   static uint8_t A = 0;
//   static uint8_t B = 0;
//   static uint8_t X = 0;
//   static uint8_t Y = 0;
//
//   static uint8_t byte_2 = 2;
//   static uint8_t byte_3 = 3;
//   static uint8_t byte_4 = 4;
//   static uint8_t byte_5 = 5;
//   static uint8_t CAN_buffer = 0;
//   static uint16_t CAN_buffer16 = 0;
//
//   static int16_t spD = 0;
//   static int holdD = 0;
//   static int16_t spS = 0;
//   static int holdS = 0;
//   static int flagg_forste = 1;
//
//   static int16_t avvikD = 0;
//   static int16_t avvikS = 0;
//   static float padragD = 0.0f;
//   static float padragS = 0.0f;
//   static uint16_t T_s = 0.1;
//   static uint16_t KpD = 0.1;
//   static uint16_t KpS = 0.1;
//   static uint16_t TiD = 1;
//   static uint16_t TiS = 1;
//
//   static uint8_t strom = 0;
///*
//   static uint16_t CCR3_Val = 1500;
//   static uint16_t CCR4_Val = 1500;
//   static PrescalerValue = 24;
//   static uint16_t periode = 20000;
//*/
//   static TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//   //static TIM_OCInitTypeDef  TIM_OCInitStructure;
//
//   static float padragT = 0.0f;
//   static uint8_t pad_up = 0;
//   static uint8_t pad_down = 0;
//
//   static float padragG = 0.0f;
//   static int16_t avtrekker = 0;
//   static uint8_t flagg_inn = 0;
//   static uint8_t flagg_ut = 1;
/////* Function definitions ----------------------------------------------------------------*/
////
/////* Define all functions here. Use the static keyword for private functions and the
//// * extern keyword for public functions. Document the function by writing what it does
//// * over the function definition.
/////**
//// * @brief  Explain what this function does here.
//// * @param  uint8_t parameter: bla bla bla... can be a value of: ...
//// * @retval Returns the number of...
//// */
////   static void Hent_SettPunkt(void){
////	   A = 0; B = 0; X = 0; Y = 0;
////	   CAN_buffer = CAN_getByteFromMessage(CAN_RX_FILTER_2, byte_4);
////	   if (CAN_buffer & 0x1)A = 1;
////	   if ((CAN_buffer>>1) & 0x1)B = 1;
////	   if ((CAN_buffer>>2) & 0x1)X = 1;
////	   if ((CAN_buffer>>3) & 0x1)Y = 1;
////
////	   if ((X > 0) && (B = 0)) {
////		spD += 1;
////		holdD = 0;
////	   }else if ((X = 0) && (B > 0)) {
////		spD -= 1;
////		holdD = 0;
////	   }else if ((X = 0) && (B = 0) && (holdD = 0)) {
////		spD = vinkelD;
////		holdD = 1;
////	   }
////	   if ((Y > 0) && (A = 0)) {
////	   		spS += 1;
////	   		holdS = 0;
////	   	   }else if ((Y = 0) && (A > 0)) {
////	   		spS -= 1;
////	   		holdS = 0;
////	   	   }else if ((Y = 0) && (A = 0) && (holdS = 0)) {
////	   	   		spS = vinkelS;
////	   	   		holdS = 1;
////	   	 }
////	   if (spS < 0)spS = 0;
////	   if (spS > 1024/2)spS = 1024/2;
////	   if (spD < 0)spD += 1024;
////	   if (spD > 1024)spD -= 1024;
////   }
//   static void GPIO_Oppstart_Enkoder(void){
//		GPIO_InitTypeDef GPIO_InitStructure;
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//
//		// GPIO setup for SPI
//		//****************************************************************************
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // PA13 CLK, PA14 MISO
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14; // PA13 CLK, PA14 MISO
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//		GPIOB->ODR = GPIOB->ODR | GPIO_Pin_13; // Setter klokkesignalet høyt fra start
//
//		//****************************************************************************
//   }
////
////   static void Vent(int tid){
////   	volatile int tim = 0;
////   	while (tim < tid)tim++;
////  }
////   static uint16_t Vinkel_Grader(uint16_t pos){
////   	uint16_t v = (uint16_t) (pos/1024)*360;
////   	return v;
////   }
/////**
//// * @brief  Explain what this function does here.
//// * @param  None
//// * @retval None
//// */
//extern void Manip_Oppstart(void){
//    GPIO_Oppstart_Enkoder();
//}
////extern void Les_Enkodere(void){
////		GPIOB->ODR &= ~GPIO_Pin_13; //1 sett klokke lav
////		Vent(t/2); //2 vent en halv periode
////		int i;
////		uint16_t k = 0;
////		uint16_t ki = 0;
////		bufferD = 0;
////		bufferS = 0;
////		for(i = 0; i < 10; i++){
////			GPIOB->ODR |= GPIO_Pin_13; //3 sett klokke høy
////			Vent(t/2); //4 vent en halv periode
////			bufferS = bufferS << 1; //5 flytt bit ett hakk
////			bufferD = bufferD << 1;
////			GPIOB->ODR &= ~GPIO_Pin_13; //6 sett klokke lav
////			if(GPIOB->IDR & GPIO_Pin_14){//7 Les innganger
////				bufferS += 0x1;
////			}
////			if(GPIOB->IDR & GPIO_Pin_15){
////				bufferD += 0x1;
////			}
////			Vent(t/2); //8 vent en halv periode
////		}
////		GPIOB->ODR |= GPIO_Pin_13; //9 sett klokke høy
////		k = bufferD;
////		ki = bufferS;
////		vinkelS = bufferS;
////		vinkelD = bufferD;
////
////}
////extern void Manip_Reguler(void){
////	Hent_SettPunkt();
////	if (flagg_forste > 0){
////		spD = vinkelD;
////		spS = vinkelS;
////		flagg_forste = 0;
////	}
////	avvikD = spD - vinkelD;
////	avvikS = spS - vinkelS;
////	if (X > 0) {
////		padragD = (float) KpD*abs(avvikD) + (KpD/TiD)*abs(avvikD)*T_s;
////	}else if (B > 0) {
////		padragD = (float) - KpD*abs(avvikD) - (KpD/TiD)*abs(avvikD)*T_s;
////	}else {
////		padragD = (float) KpD*avvikD + (KpD/TiD)*avvikD*T_s;
////	}
////	if (Y > 0) {
////		padragS = (float) KpS*abs(avvikS) + (KpS/TiS)*abs(avvikS)*T_s;
////	}else if (A > 0) {
////		padragS = (float) - KpS*abs(avvikS) - (KpS/TiS)*abs(avvikS)*T_s;
////	}else {
////		padragS = (float) KpS*avvikS + (KpS/TiS)*avvikS*T_s;
////	}
////	VESC_setDutyCycle(ESC_ID_10, padragD);
////	VESC_setDutyCycle(ESC_ID_11, padragS);
////}
////extern void  Manip_Teleskop(uint8_t strom){
////	pad_down = 0; pad_up = 0;
////	CAN_buffer = CAN_getByteFromMessage(CAN_RX_FILTER_2, byte_5);
////	if ((CAN_buffer>>5) & 0x1)pad_up = 1;
////	if ((CAN_buffer>>6) & 0x1)pad_down = 1;
////
////	if ((pad_up > 0) && (pad_down = 0) && (strom < 25)){
////		padragT = 0.84f;
////	}else if ((pad_up = 0) && (pad_down > 0) && (strom < 25)) {
////		padragT = -0.84f;
////	}else {
////		padragT = 0.0f;
////	}
////	VESC_setDutyCycle(ESC_ID_9, padragT);
////}
//extern void Manip_Foresporsel_Strom(void){
//	Interface_VESC_requestData(motor_strom_teller, CAN_PACKET_GET_CURRENT);
//
////	/* Increment counter. */
////	if(motor_strom_teller < totalt_antall_motorer){
////		motor_strom_teller++;
////	} else {
////		motor_strom_teller = 9;
////	}
//}
//extern void Manip_Hent_Strom(void){
//	/* Read messages from CAN receive buffer */
//		uint8_t current = CAN_getByteFromMessage(fmi_manip_motor_curr9, 1);
//		strom = (uint8_t) current;
//}
//extern void  Manip_Grip_F(void){
//	Manip_Foresporsel_Strom();
//	controller_manip = Interface_readController();
//	Manip_Hent_Strom();
//
//	if ((strom < 25) && (controller_manip[7] > 0)){
//		padragG = 0.50f;
//	}else if ((strom < 25) && (controller_manip[7] < 0)){
//		padragG = -0.50f;
//	}else {
//		padragG = 0.0f;
//	}
//	VESC_setDutyCycle(ESC_ID_9, padragG); //Sender motorpaadrag
//}
//
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
