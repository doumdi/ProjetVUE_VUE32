/*************************************************************************
 *
 *  BMS.h
 *
 **************************************************************************/
// Todo: Proper header

#include "BQ_Branch.h"

#ifndef _BMS_H_
#define _BMS_H_

// Includes
#include "GenericTypeDefs.h"

//#include <p32xxxx.h>
//#include <sys/kmem.h>
//#include "plib.h"

#define TENSION_MAX_CELL					3600 			// Tension maximale pour activer le fet ne pas dépasser 4200 Doit être ajusté selon le type de cellule
#define TENSION_MIN_CELL                    2000
#define GAP_OVER_MIN_CELL					100			// Gap de tension avec la plus basse tension
#define TENSION_START_FET					3400		// Tension minimal pour activer le FET
#define TENSION_MAX_OPEN_CONTACTOR          3650        // Tension ou les contacteurs ouvrent

#define TEMPERATURE_MAX_CELL                500                     //Temperature maximale des cellules 50.0°C
#define TEMPERATURE_MAX_RES                 900                     //Temperature maximale des résistances  90.0°C

////////////////////////////////////////////////////////////////////////////////
// Defines from CANFunctions.h
////////////////////////////////////////////////////////////////////////////////

#define NBTENTATIVE 1000


//----------------------------------------
//#define NB_MESSAGE_BY_BRANCH 10
#define NB_MESSAGE_FIFO		32
//BYTE CAN1MessageFifoArea[4 * NB_MESSAGE_FIFO * 16];

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

#define CG_ID_BMS_CELLGROUP_VOLT1_TEMP 0x00
#define CG_ID_BMS_CELLGROUP_VOLT2 0x01

BOOL ErrorStateFlag;



////////////////////////////////////////////////////////////////////////////////

typedef struct {
	unsigned char address;					//Numero du bms
	unsigned char branchAddress;				//Addresse de la branche
	unsigned char CANAddress;
}BMSAddress;



typedef struct{
	unsigned char value;
	unsigned char changed;
}charM;

BMSAddress bmsAddress;

//États possible du BMS
/**	InitSleep=0, Sleep=1, Balance=2, Monitor=3, InitBQ=4 InitPeripheral=5, Test=6, WakeUp=7, WaitStabilise=8 WaitStabiliseTemp=9**/
enum eStates{InitBQ,InitSleep,Sleep,Balance,Monitor,InitPeripheral,Test,WakeUp,WaitStabiliseTension,WaitStabiliseTemp,ProblemDetected};
//extern enum eStates m_state, lastState;

enum eStates m_state, lastState;

unsigned int tensionMaxCell;
unsigned int gapOverMinCell;
unsigned int tensionStartFet;
unsigned int tensionMinCell;

unsigned int tensionMaxOpenContactor;

unsigned int temperatureMaxCell;
unsigned int temperatureMaxRes;

void delayTime(unsigned int time);


////////////////////////////////////////////////////////////////////////////////

#endif // _BMS_H_
