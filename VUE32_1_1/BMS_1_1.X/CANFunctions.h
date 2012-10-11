/**********************************************************************
* © 2007 Microchip Technology Inc.
*
* FileName:        CANFunctions.h	 
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       PIC32
* Compiler:        MPLAB® C32 
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Incorporated ("Microchip") retains all ownership and 
* intellectual property rights in the code accompanying this message and in all 
* derivatives hereto.  You may use this code, and any derivatives created by 
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes 
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO 
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A 
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S 
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER 
* IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY), 
* STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL, 
* PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF 
* ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN 
* ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT 
* ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO 
* THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO 
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and 
* determining its suitability.  Microchip has no obligation to modify, test, 
* certify, or support the code.
************************************************************************/

#ifndef _CAN_FUNCTIONS_H_
#define _CAN_FUNCTIONS_H_

#include "GenericTypeDefs.h"
//#include "h\Explorer16.h"
#include <p32xxxx.h>
#include <sys/kmem.h>
#include "bq_branch.h"
#include "plib.h"


#define NBTENTATIVE 1000


//----------------------------------------
//#define NB_MESSAGE_BY_BRANCH 10
#define NB_MESSAGE_FIFO		32
BYTE CAN1MessageFifoArea[4 * NB_MESSAGE_FIFO * 16];

//----------------------------------------
// DEFINE CAN1
#define CAN1_VALEUR_FILTRE0			0x00000
#define CAN1_VALEUR_FILTRE1			0x00000

#define CAN1_VALEUR_MASQUE0			0x3FC0000 //Filtre seulement sur le SID
#define CAN1_VALEUR_MASQUE1			0x3FC0000

#define CAN1_TX_FIFO_PRIORITAIRE		CAN_CHANNEL0
#define CAN1_TX_FIFO_NORMAL			CAN_CHANNEL1

#define CAN1_RX_FIFO_PRIORITAIRE		CAN_CHANNEL2
#define CAN1_RX_FIFO_PRIORITAIRE_EVENT          CAN_CHANNEL2_EVENT
#define CAN1_RX_FIFO_NORMAL			CAN_CHANNEL3
#define CAN1_RX_FIFO_NORMAL_EVENT		CAN_CHANNEL3_EVENT





#define CAN1_BRPVAL	0x7			/* CAN speed select - 0x7 for 500kbps, 0xF for 250Kbps, 0x3 for 1Mbps 		*/
#define CAN2_BRPVAL 0x7			/* Use same speed for both CAN modules. Time quanta per bit is set to 10.	*/
								/* See function CAN1Init() and CAN2Init().*/

#define ADDR_CAN_ODB 0x00
/**
* @brief Id of each possible variable, the same variable can be implemented by many devices
* @author Pascal-Frederic St-Laurent
*/
//
//enum CANResourceId {
//
//	E_ID_BMS_BOARD_TEMP_BALANCING = 0x14,
//	E_ID_BMS_CELL_GROUP1 = 0x15,
//	E_ID_BMS_CELL_GROUP2 = 0x16,
//	E_ID_BMS_CELLGROUP_VOLT1_TEMP_DEVICE = 0x17,
//	E_ID_BMS_CELLGROUP_VOLT2 = 0x18,
//	E_ID_BMS_OPEN_CONTACT = 0x19,
//	E_ID_BMS_STATE = 0x1A
//};

/**
* @brief This Enum contains every implemented trame type.
* @author Pascal-Frederic St-Laurent
*/



/*int CAN1Init(void);
void CAN1RxMsgProcess(void);
void CAN1TxSendLEDMsg(void);

void CANSendAck();

void CAN1RxPrioritaireMsgProcess();
void CAN1RxNormalMsgProcess();


void CANTransmetChienDeGarde(void);
void CANTransmetBranch(Branch *branch,unsigned short address);
void CANTransmetStatus();
void CANTransmetTempBal(Branch* branch,unsigned short address);
void CANTransmetTensionMinMax(Branch* branch,unsigned short address, unsigned char send);
void CANTransmetTension(Branch* branch,unsigned short address);


void CANTransmetOpenContactor();
void PadMessageWithZeros(CANTxMessageBuffer * message,unsigned char start, unsigned char end);


CANTxMessageBuffer* CANCGetMessageNormal(void);
CANTxMessageBuffer* CANCGetMessageHighest(void);
void CANCompleteMessageAndSendNormal(CANTxMessageBuffer * message,unsigned short address);
void CANSendNormalMessage(CANTxMessageBuffer * message,unsigned short address, unsigned char DLC);
void CANCompleteMessageAndSendHighest(CANTxMessageBuffer * message,unsigned short address);*/

#endif
