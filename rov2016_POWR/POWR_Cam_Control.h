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


//#define Dimming_Value							0x78		//

/* Exported function prototypes --------------------------------------------------------*/

/* Declare global functions here, preferrably with comments to explain the functions.
 */

extern void Cam_Control_Init(void);
extern void setCAM_POS(void);// @brief To change the Camera Angle
											  // @param  uint8_t Cam_message goes frome 0-10 ---> To the angle 60->90->120
