	/**
  **************************************************************************************
  * @file    main.c
  * @author  Regulering og Manipulatorstyring
  * @version V1.0
  * @date    09-February-2016
  * @brief   Start. Init
  **************************************************************************************
  */

/* -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
 * Please keep lines of comments and code less than 90 characters long.
 * -.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.,-.
 */


/* Include------------------------------------------------------------------------------*/

/* Include .h files from other modules here.*/

#include "stm32f10x_gpio.h" // Gpio oppsett
#include "stm32f10x.h"    // h-file specific to the microcontroller. Allways include this.
#include "stm32f10x_can.h"
#include "POWR_Switch.h"
#include "POWR_Canbus.h"
#include "POWR_Current.h"
#include "semihosting.h"
#include "POWR_define_global_vars.h"
#include "POWR_Cam_Control.h"
#include "POWR_SysTick.h"

/* Local variables */



 uint8_t message = 0b00000100;

int main(void)
{

	initReleStyring();		//Oppsett av GPIO
	init_TIM4();			//Oppsett av Timer
	CAN_init();				//Oppsett av Can-Bus

	SysTick_init();			//Oppsett av SysTick
	//Cam_Control_Init();		//Oppsett av Cam_Control
	//setCAM_POS();			//PWM signal ut


	fmi_topside_powr_ctrl = CAN_addRxFilter(TOP_POWR_CTRL);
	fmi_manip_motor_curr_9 = CAN_addRxFilter(VESC_CURRENT_MANIP_GRIP); //manip.c
	fmi_manip_motor_curr_10= CAN_addRxFilter(VESC_CURRENT_MANIP_RULL);//manip.c
	fmi_manip_motor_curr_11= CAN_addRxFilter(VESC_CURRENT_MANIP_STAMP);//manip.c
	fmi_manip_motor_curr_12= CAN_addRxFilter(VESC_CURRENT_MANIP_TELE);//manip.c


	setPOWR_UPDATE(message);
	//Motor konstant PÅ!
	//GPIOB->ODR |= GPIO_Pin_3;



	while(1){



	}



}
