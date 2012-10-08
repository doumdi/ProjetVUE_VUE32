/*************************************************************************
 *
 *  BMS.h
 *
 **************************************************************************/
// Todo: Proper header

#ifndef _BMS_H_
#define _BMS_H_

#define BUFFER_MOY  1

//Branch
#define NB_DEVICE_BY_BRANCH					2			// Number Max of device/branch

//Device
#define NB_CELL_BY_DEVICE					4			// Number of cell/device

#define TENSION_MAX_CELL					3600 			// Tension maximale pour activer le fet ne pas dépasser 4200 Doit être ajusté selon le type de cellule
#define TENSION_MIN_CELL                    2000
#define GAP_OVER_MIN_CELL					100			// Gap de tension avec la plus basse tension
#define TENSION_START_FET					3400		// Tension minimal pour activer le FET
#define TENSION_MAX_OPEN_CONTACTOR          3650        // Tension ou les contacteurs ouvrent

#define TEMPERATURE_MAX_CELL                500                     //Temperature maximale des cellules 50.0°C
#define TEMPERATURE_MAX_RES                 900                     //Temperature maximale des résistances  90.0°C

typedef struct {
	unsigned char address;					//Numero du bms
	unsigned char branchAddress;				//Addresse de la branche
	unsigned char CANAddress;
}BMSAddress;

typedef struct{
	unsigned int value;
	unsigned char changed;
}intM;

typedef struct{
	unsigned char value;
	unsigned char changed;
}charM;

BMSAddress bmsAddress;

unsigned int tensionMaxCell;
unsigned int gapOverMinCell;
unsigned int tensionStartFet;
unsigned int tensionMinCell;

unsigned int tensionMaxOpenContactor;

unsigned int temperatureMaxCell;
unsigned int temperatureMaxRes;

//États possible du BMS
/**	InitSleep=0, Sleep=1, Balance=2, Monitor=3, InitBQ=4 InitPeripheral=5, Test=6, WakeUp=7, WaitStabilise=8 WaitStabiliseTemp=9**/
enum eStates{InitSleep,Sleep,Balance,Monitor,InitBQ,InitPeripheral,Test,WakeUp,WaitStabiliseTension,WaitStabiliseTemp,ProblemDetected};

#endif // _BMS_H_
