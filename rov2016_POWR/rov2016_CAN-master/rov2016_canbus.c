///**
// **************************************************************************************
// * @file    CAN_metoder.c
// * @author  Sivert Sliper, Stian Soerensen
// * @version V1.0
// * @date    3-February-2016
// * @brief   This file contains all the functions for the CAN peripheral.
// **************************************************************************************
// */
//
///* Include------------------------------------------------------------------------------*/
//#include "stm32f10x.h"
//#include "rov2016_canbus.h"
//#include "stm32f10x_can.h"
//#include "stm32f10x_rcc.h"
//#include "stm32f10x_gpio.h"
//#include "misc.h"
//
///* Macro */
////#define DEBUG_MODE
//
///* Static Function declarations --------------------------------------------------------*/
//
///* Private variables -------------------------------------------------------------------*/
//static CanRxMsg RxMsg;
//static CanTxMsg TxMsg = {0};
//static uint8_t rx_messages = 0; // Counter for the number of new messages received.
//static uint8_t TransmitMailbox = 0; // Used for transmitting messages.
//static uint8_t FMI_counter = 0;
//
///* Array for incoming messages, messages are stored in a row according to filter match
// * indicator(FMI).
// */
//static uint8_t Rx_Array[16][8];
//
///* Function definitions ----------------------------------------------------------------*/
//
///**
// * @brief  Configures the CAN-Controller peripheral for 500 kbps communication.
// * 		   Also configures Rx filters according to ID's specified in "can_metoder.h"
// * @param  None
// * @retval None
// */
//void CAN_Config(void){
//	CAN_InitTypeDef       	CAN_InitStructure;
//	CAN_FilterInitTypeDef 	CAN_FilterInitStructure;
//	NVIC_InitTypeDef		NVIC_InitStructure;
//
//
//	/* Enable clocks ********************************************************************/
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	/* CAN GPIOs configuration. *********************************************************/
//
//
//	/* Configure CAN RX and TX pins. */
//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	/* PA11 - Rx should be configured as open drain. */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	/* CAN receiver light. */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//
//
//
//	/* CAN configuration ***************************************************************/
//
//	/* CAN register init */
//	CAN_DeInit(CAN1);
//
//	/* CAN cell init */
//	CAN_InitStructure.CAN_TTCM = DISABLE;
//	CAN_InitStructure.CAN_ABOM = DISABLE;
//	CAN_InitStructure.CAN_AWUM = DISABLE;
//	CAN_InitStructure.CAN_NART = DISABLE;
//	CAN_InitStructure.CAN_RFLM = DISABLE;
//	CAN_InitStructure.CAN_TXFP = DISABLE;
//	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
//	CAN_InitStructure.CAN_SJW = CAN_SJW_4tq;
//
//	/* CAN Baudrate = 500 kbps (CAN clocked at 9 MHz)
//	 * se kapittel 4.1.3 i oppgaven og ref.manual RM0316 seksjon 31.7 */
//	CAN_InitStructure.CAN_BS1 = CAN_BS1_11tq;
//	CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
//	CAN_InitStructure.CAN_Prescaler = 4; // 36MHz/4 = 9 MHz
//
//	/* CAN filter init *****************************************************************/
//	CAN_FilterInitStructure.CAN_FilterNumber = 0; // [0...13]
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
//
//	/* Setting hardware filtering of incomming messages. Using 16-bit list mode
//	 * allows 4 11-bit ID's to be compared. The mask-filters are also used as
//	 *  ID's.
//	 *
//	 * => 4 ID's per filter => maximum 4x14 = 56 message ID's total.
//	 *
//	 * Mapping: StdID[10:0]-RTR-IDE-EXID[17-15] ref. figure 391 RM0316.
//	 * */
//
//	/*Filter Match Index 0*/
//	CAN_FilterInitStructure.CAN_FilterIdLow = 		  (CAN_RX_FILTER_0 << 5) \
//			| (CAN_RTR_DATA << 4)	 \
//			| (CAN_ID_STD << 3);
//	/*Filter Match Index 1*/
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 	  (CAN_RX_FILTER_1 << 5) \
//			| (CAN_RTR_DATA << 4)	 \
//			| (CAN_ID_STD << 3);
//
//	/*Filter Match Index 2*/
//	CAN_FilterInitStructure.CAN_FilterIdHigh = 		  (CAN_RX_FILTER_2 << 5) \
//			| (CAN_RTR_DATA << 4)	 \
//			| (CAN_ID_STD << 3); // Filt. no. 2
//
//	/*Filter Match Index 3*/
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 	  (CAN_RX_FILTER_3 << 5) \
//			| (CAN_RTR_DATA << 4)	 \
//			| (CAN_ID_STD << 3);
//
//
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0; // Rx-buffer
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
//
//	CAN_FilterInit(&CAN_FilterInitStructure);
//
//	/* Enable FIFO 0 message pending Interrupt */
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
//
//	/* NVIC configuration **************************************************************/
//	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//
//	/* Initialize a standard transmit message */
//
//	TxMsg.IDE = CAN_ID_STD;
//	TxMsg.StdId = 0x301;
//	TxMsg.ExtId = 0x00;
//	TxMsg.RTR = CAN_RTR_DATA;
//	TxMsg.IDE = CAN_ID_STD;
//	TxMsg.DLC = 1;
//	TxMsg.Data[0] = 0x0;
//}
//
///**
// * @brief  	This function handles CAN1 RX0 request by storing the received message
// * 			in an array according to the filter match index and message size.
// *
// * 			Received messages are indicated by flipping LED 3.
// *
// * 			The interrupt pending bit is automatically cleared by hardware.
// * @param  	None
// * @retval 	None
// */
//void CAN1_RX1_IRQHandler(void){
//	CAN_Receive(CAN1, CAN_FIFO0, &RxMsg);
//	uint8_t bytes = RxMsg.DLC;
//
//	if (bytes == 0) return; // Return if message is empty.
//
//	while(bytes-->0){
//		Rx_Array[RxMsg.FMI][bytes-1] = RxMsg.Data[bytes-1];
//	}
//
//	/* Increment message received counter */
//	rx_messages++;
//
//	/* Indicating message received.*/
//	GPIOC->ODR ^= GPIO_Pin_13; // Flip receive-LED.
//}
//
///**
// * @brief  Returns the number of unprocessed messages.
// * @param  None
// * @retval The number of unprocessed messages (uint8_t).
// */
//uint8_t CAN_getRxMessages(void){
//	return rx_messages;
//}
//
///**
// * @brief  	Returns the specified byte from the Rx array.
// * @param  	uint8_t filter_number: 	Specify which filter index the wanted
// * 									message belongs to.
// * @param	uint8_t byte_number:	Specify where the wanted byte is in the
// * 									data field of the received message.
// * @retval 	The specified byte.
// */
//uint8_t CAN_getByteFromMessage(uint8_t filter_number, uint8_t byte_number){
//	rx_messages--;
//	return Rx_Array[filter_number][byte_number];
//}
//
///**
// * @brief  Transmit byte
// * @param  None
// * @retval The number of unprocessed messages (uint8_t).
// */
//void CAN_transmitByte(uint16_t StdId, uint8_t data){
//	/* Toggle status LED */
//	//GPIOE->ODR ^= CAN_TX_LED << 8;
//
//	/* Configure the message to be transmitted. */
//	TxMsg.StdId = StdId;
//	TxMsg.IDE = CAN_ID_EXT;
//	TxMsg.DLC = 1;
//	TxMsg.Data[0] = data;
//
//	/* Put message in Tx Mailbox and store the mailbox number. */
//	TransmitMailbox = CAN_Transmit(CAN1, &TxMsg);
//
//	/* Wait on Transmit */
//	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok));
//}
//
//
///**
// * @brief  	Transmit buffer
// * @param  	Id 		- ID of message to be sent.
// * 			buffer 	- Pointer to a buffer containing bytes to be sent.
// * 			length	- Length of packet to be sent. (in bytes).
// * 			Id_Type - Identifier type, can be:
// * 							CAN_ID_STD - Standard 11 bit identifier.
// * 							CAN_ID_EXT - Extended 31 bit identifier.
// * @retval 	None
// */
//extern void CAN_transmitBuffer(uint32_t Id, uint8_t* buffer, uint8_t length, uint8_t Id_Type){
//	/* Parameter check. */
//	if(length > 8){
//		return;
//	}
//
//	//GPIOE->ODR ^= CAN_TX_LED << 8;
//
//	/* Prepare message to be sent */
//	TxMsg.IDE = Id_Type;
//	TxMsg.RTR = CAN_RTR_DATA;
//	TxMsg.DLC = length;
//
//	/* Load ID into message. */
//	if(Id_Type == CAN_ID_EXT){
//		TxMsg.StdId = (uint16_t)(Id >> 18); // Top 11 bits of the 31 bit identifier
//		TxMsg.ExtId = (uint32_t)(Id & 0x00003FFFF); // Bottom 18 bits of the identifier.
//	} else {
//		TxMsg.StdId = Id;
//		TxMsg.ExtId = 0;
//	}
//
//	/* Load data into message. */
//
//	volatile uint8_t i;
//	for(i=0; i<length; i++){
//		TxMsg.Data[i] = buffer[i];
//	}
//
//	/* Put message in Tx Mailbox and store the mailbox number.
//	 * Stall if no mailbox is available.
//	 */
//	TransmitMailbox = CAN_TxStatus_NoMailBox;
//	while(TransmitMailbox == CAN_TxStatus_NoMailBox){
//		TransmitMailbox = CAN_Transmit(CAN1, &TxMsg);
//	}
//
//	/* Wait for transmit complete.*/
//	//	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok));
//
//}
//
///**
// * @brief  Transmit SENSOR_AN_RAW package.
// * @param  None
// * @retval The number of unprocessed messages (uint8_t).
// */
//void CAN_transmit_AN_RAW(void){
//	/* Toggle status LED */
//	//GPIOE->ODR ^= CAN_TX_LED << 8;
//
//	/* Configure the message to be transmitted. */
//	TxMsg.StdId = SENSOR_AN_RAW;
//	TxMsg.DLC = 8;
//	TxMsg.IDE = CAN_ID_STD;
//	TxMsg.RTR = CAN_RTR_DATA;
//
//	TxMsg.Data[0] = (uint8_t) (ADC4_getChannel(0) & 0xFF);
//	TxMsg.Data[1] = (uint8_t) (ADC4_getChannel(0) >> 8);
//	TxMsg.Data[2] = (uint8_t) (ADC1_getChannel(1) & 0xFF);
//	TxMsg.Data[3] = (uint8_t) (ADC1_getChannel(1) >> 8);
//	TxMsg.Data[4] = (uint8_t) (ADC1_getChannel(2) & 0xFF);
//	TxMsg.Data[5] = (uint8_t) (ADC1_getChannel(2) >> 8);
//	TxMsg.Data[6] = (uint8_t) (ADC1_getChannel(3) & 0xFF);
//	TxMsg.Data[7] = (uint8_t) (ADC1_getChannel(3) >> 8);
//
//	/* Put message in Tx Mailbox and store the mailbox number. */
//	TransmitMailbox = CAN_Transmit(CAN1, &TxMsg);
//
//	/* Wait for Transmit */
//	while(CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok);
//}
//
///**
// * @brief  Transmit acceleration measurements in all 3 axes.
// * @param  Pointer to an array of dimension 6 containing measurements.
// * @retval None
// */
//void CAN_transmitAcceleration(int8_t *acc_array){
//	/* Toggle status LED */
//	//GPIOE->ODR ^= CAN_TX_LED << 8;
//
//	/* Configure the message to be transmitted. */
//	TxMsg.StdId = SENSOR_ACCELERATION;
//	TxMsg.DLC = 6;
//
//	uint8_t i = 0;
//	for(i=0;i<6;i++) TxMsg.Data[i] = *acc_array++;
//
//	/* Put message in Tx Mailbox and store the mailbox number. */
//	TransmitMailbox = CAN_Transmit(CAN1, &TxMsg);
//
//	/* Wait on Transmit */
//	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok));
//}
//
///**
// * @brief  Transmit 4 quaternions as 4 int16_t split into 8 bytes.
// * @param  int16_t q0, q1, q2, q3
// * @retval None.
// */
//extern void CAN_transmitQuaternions(int16_t q0, int16_t q1, int16_t q2, int16_t q3){
//	/* Toggle status LED */
//	//GPIOE->ODR ^= CAN_TX_LED << 8;
//
//	/* Configure the message to be transmitted. */
//	TxMsg.StdId = SENSOR_AHRS_QUATERNIONS;
//	TxMsg.DLC = 8;
//
//	TxMsg.Data[0] = (uint8_t)(q0 >> 8u);
//	TxMsg.Data[1] = (uint8_t)(q0 & 0xFF);
//	TxMsg.Data[2] = (uint8_t)(q1 >> 8u);
//	TxMsg.Data[3] = (uint8_t)(q1 & 0xFF);
//	TxMsg.Data[4] = (uint8_t)(q2 >> 8u);
//	TxMsg.Data[5] = (uint8_t)(q2 & 0xFF);
//	TxMsg.Data[6] = (uint8_t)(q3 >> 8u);
//	TxMsg.Data[7] = (uint8_t)(q3 & 0xFF);
//
//	/* Put message in Tx Mailbox and store the mailbox number. */
//	TransmitMailbox = CAN_Transmit(CAN1, &TxMsg);
//
//	/* Wait on Transmit */
//	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CAN_TxStatus_Ok));
//}
