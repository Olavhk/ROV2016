/**
  **************************************************************************************
  * @file    h-file_template
  * @author  Olav H Karstensen
  * @version V01
  * @date    09-Mars-2016
  * @brief   This file contains macros and extern function definitions for
  			 c-file_template.c.
  **************************************************************************************
  */

/* Macro -------------------------------------------------------------------------------*/

#define Motor_Switch							0b00000001 	//1byte
#define Light_Switch							0b00000010 	//1byte
#define CoolingFan_Switch						0b00000100 	//1byte
#define Dimming_Value							0x78		//

/* Exported function prototypes --------------------------------------------------------*/

/* Declare global functions here, preferrably with comments to explain the functions.
 */

extern void initReleStyring(void); //Init GPIO
extern void init_TIM4(void); // Init timer to PWM to the LED Dimmer
extern void setPOWR_UPDATE(uint8_t message); // Update all outputs and the dimming value
//extern void TIM4_dutyCycle(uint8_t dimming);

//extern void setTIM4_dutyCycle(uint8_t dimming);// @brief  Dimmming the light on the Rov, Changing the duty cycle
											  // @param  uint8_t dimming goes frome 0-10 ---> 25% -100% Dimming of the light
