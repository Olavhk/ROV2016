///**
//  **************************************************************************************
//  * @file    ADC_metoder.h
//  * @author  Sivert Sliper, Stian Soerensen
//  * @version V01
//  * @date    03-February-2016
//  * @brief   This file contains local variables and macros for CAN_metoder.c
//  **************************************************************************************
//  */
//
///* Include -----------------------------------------------------------------------------*/
//
///* Private macro -----------------------------------------------------------------------*/
//#define CAN_ID_TYPE_STD						0u
//#define CAN_ID_TYPE_EXT						4u
//
///* ID list, RANGE = [0...0x7FF] *********************************************************/
//typedef enum {
//	CAN_PACKET_SET_DUTY = 0,
//	CAN_PACKET_SET_CURRENT,
//	CAN_PACKET_SET_CURRENT_BRAKE,
//	CAN_PACKET_SET_RPM,
//	CAN_PACKET_SET_POS,
//	CAN_PACKET_FILL_RX_BUFFER,
//	CAN_PACKET_FILL_RX_BUFFER_LONG,
//	CAN_PACKET_PROCESS_RX_BUFFER,
//	CAN_PACKET_PROCESS_SHORT_BUFFER,
//	CAN_PACKET_STATUS,
//	CAN_PACKET_GET_CURRENT,
//	CAN_PACKET_GET_RPM,
//	CAN_PACKET_GET_MOSFET_TEMP
//} CAN_PACKET_ID;
//
//#define TOP_BASE 						0x200
//#define TOP_XBOX_CTRLS					TOP_BASE
//#define TOP_XBOX_AXES					(TOP_BASE + 1)
//#define TOP_REG_TRANS_PARAM				(TOP_BASE + 3)
//#define TOP_REG_ROT_PARAM 				(TOP_BASE + 4)
//
//
//#define SENSOR_BASE 					0x300
//#define SENSOR_AN_RAW					SENSOR_BASE
//#define SENSOR_PROCESSED_DATA			(SENSOR_BASE + 1)
//#define SENSOR_ACCELERATION				(SENSOR_BASE + 2)
//#define SENSOR_DEPTH_TEMP				(SENSOR_BASE + 3)
//#define SENSOR_LEAKAGE_ALARM			(SENSOR_BASE + 4)
//#define SENSOR_AHRS_QUATERNIONS			(SENSOR_BASE + 5)
//#define SENSOR_ALIVE					(SENSOR_BASE + 6)
//#define SENSOR_AHRS						(SENSOR_BASE + 7)
//#define SENSOR_CURR_TEST				(SENSOR_BASE + 8)
//
//
//#define POWR_BASE 						0x400
//#define POWR_STATUS					   	POWR_BASE			//Update Swith, light, fan. motor.
//#define POWR_UPDATE						(POWR_BASE + 1)
//#define POWR_ALIVE                      (POWR_BASE + 2)
////#define POWR_ENCODER_ONE_AND_TW0		(POWR_BASE + 2)		//Sender Manipulatorstilling
////#define POWR_CAM_PWM					(POWR_BASE + 3)
////#define POWR_PWM_MANIP_ONE_AND_TWO		(POWR_BASE + 4)     // PWM
////#define POWR_ENCODER_ONE				(POWR_BASE + 5)
////#define POWR_ENCODER_TOW				(POWR_BASE + 6)
////#define POWR_CAM_PWM					(POWR_BASE + 7)
//
//#define MOTOR_GRIP						0x1
//#define MOTOR_DREI						0x2
//#define MOTOR_STAMP						0x3
//#define MOTOR_TELESKOP						0x4
//
//#define ESC_BASE						0x500
//#define ESC_1_manip							ESC_BASE
//
//
///* Exported function prototypes --------------------------------------------------------*/
//void CAN_Config(void);
//void CAN_IT_Config(void);
//uint8_t CAN_getByteFromMessage(uint8_t filter_number, uint8_t byte_number);
//uint8_t CAN_getRxMessages(void);
//uint16_t ADC1_getChannel(uint8_t channel);
//uint16_t ADC4_getChannel(uint8_t channel);
//extern void CAN_transmitQuaternions(int16_t q0, int16_t q1, int16_t q2, int16_t q3);
//extern void CAN_transmitBuffer(uint32_t Id, uint8_t* buffer, uint8_t length, uint8_t Id_Type);
