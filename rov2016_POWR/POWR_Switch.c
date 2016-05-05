
/**
**************************************************************************************
* @file    c-fil_template.c
* @author  Olav H Karstensen
* @version V1.0
* @date    9-Mars-2016
* @brief   To turn on\off Mosfet tru Can-bus.
**************************************************************************************
*/

/* -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
* To tur On\Off Motor, light, Coolingfan in the Rov.
* -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
*/
/* Include------------------------------------------------------------------------------*/

/* Include .h files from other modules here.*/

#include "stm32f10x_gpio.h" // Gpio oppsett
#include "stm32f10x.h"    // h-file specific to the microcontroller. Allways include this.
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "POWR_Switch.h"

/* Global variables --------------------------------------------------------------------*/
/* If needed, include header file for extern declarations of global variables here.
* Try to avoid global variables. Using getters and setters is usually a better
* alternative.
*/


/* Private Function Prototypes ----------------------------------------------------------*/

	/* Put prototypes for static functions here.
	* Private/static functions can only be called from within this c-file. Use static
	* for all functions that does not need to be used by an outside program.
	*/

/* Private variables -------------------------------------------------------------------*/
/* Put static/private variables here. These variables are only accessible from this
* .c-file.
*/
	static uint8_t dimming ;



/*Public Functions --------------------------------------------------------------------*/


/* @brief  Init GPIO Pins GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
*/
extern void initReleStyring(void){
	// Settte opp GPIO pinnene til Mosfet styrings signalene.
	//Konfiguering

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // Klokketilgang il AFIO-perifermodul
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // Remap JTDO og JNTRST til PB3 og PB4

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	// Initialisering, laste ned kofigurasjonen til modulen
	GPIO_Init(GPIOB, &GPIO_InitStructure);



}

/* init_TIM4()
 @breif Initialize TIM4 as a PWM output.
 	 Output pin PB8.
 	 The p.w.m. is to change the brightness of the LED on the ROV.
 	 We need a Voltage divider to secure the dimming module.(Type:LDU1416S1000)
 	-
 */
extern void init_TIM4(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // For TIM4
	TIM_OCInitTypeDef TIM_OCInitStructure; // For TIM_OC
	GPIO_InitTypeDef GPIO_InitStructure; // for GPIOD

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // Gi tilgang til klokken

	// Innstillinger for GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); // Gi tilgang til klokken
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; // Pin PB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // Alternativ funksjon
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 10MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure); // Laster ned innstillingene

	// Innstillinger for TIM4
	/* Dette gir ~100Hz  */
	TIM_TimeBaseStructure.TIM_Period = 1000000/100 - 100; // ~100 Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 72; // Preskalering av TIM klokke
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; // Sette clock divider
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // Telleretning
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); // Laster ned innstillingene

	// Innstillinger for TIM_OC
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM modus
	/* Aktiverer sammenligning med verdien i Capture Compare Register */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //
	TIM_OCInitStructure.TIM_Pulse = ((1000000/100)-100)/2; // 50% duty cycle.
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // Polaritet

	TIM_OC3Init(TIM4, &TIM_OCInitStructure);

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM4, ENABLE);
	/* TIM4 aktiveres ikke før kjør-kommando mottas.Se SysTick_Handler().*/
	TIM_Cmd(TIM4, ENABLE);
} // end TIM4_init()



/**
* @brief  To turn the Motorswitch , Light, CoolingFan ;Mosfet ON\OFF
* @param  uint8_t parameter: 0 = Off 0 < ON
* @retval No return
*/
extern void setPOWR_UPDATE(uint8_t message){

	//MotorSwitch
	if(message & Motor_Switch)GPIOB->ODR |= GPIO_Pin_3;
	else GPIOB->ODR &= ~GPIO_Pin_3;

	//LightSwitch
	if(message & Light_Switch)GPIOB->ODR |= GPIO_Pin_4;
	else GPIOB->ODR &= ~GPIO_Pin_4;

	//CoolingFanSwitch
	if(message & CoolingFan_Switch)GPIOB->ODR |= GPIO_Pin_5;
	else GPIOB->ODR &= ~GPIO_Pin_5;
	//Light Dimming
	if(message & Dimming_Value){
		dimming = (message & Dimming_Value) >>3 ;

		if(dimming<=10) TIM4->CCR3 = (2475 + 742*dimming);
	}
}	//END setPOWR_UPDATE

