/*************************************************************************
 *
 *   
 *
 *    File name   :  branch.h
 *    Description :  Méthodes utilisées pour controler une branche en série de BQ
 *
 *    History :
 *    1. Date        : 	11-12-2011
 *       Author      : 	Joel Viau Le meilleur
 *       Description : 
 *
 **************************************************************************/

#include "BQ.h"
#include "BQ_com.h"

#ifndef BRANCHE_H
#define BRANCHE_H

#define BUFFER_MOY  1

//Branch
#define NB_DEVICE_BY_BRANCH					2			// Number Max of device/branch

//Device
#define NB_CELL_BY_DEVICE					4			// Number of cell/device


typedef struct{
	unsigned int value;
	unsigned char changed;
        unsigned char test[3];
}intM;

//Data for cellule
typedef struct{
	intM tension;					//Tension de la cellule
	intM charge;					//La charge dans la cellule
	unsigned char balanceActive;	//Le balancement est en fonction
        unsigned char test[3];
        unsigned int bufferTension[BUFFER_MOY];
}Cell;
//Data for device
typedef struct{
	byte status;			//General Status
	byte alert;				//Alert
	byte fault;				//Fault
	byte fetStatus;			//Le status des FETS
	byte cov_fault;			//Cell Over Voltage
	byte cun_fault;			//Cell Under Voltage
	byte address;			//address
        byte test;			//test
	Cell cellTable[NB_CELL_BY_DEVICE];	//Cellules data
	intM temperature1;
	intM temperature2;
	intM temperature3;
}Device;

//Data for branch
typedef struct {
	unsigned char id;							//Id de la branche
        unsigned char test;
	unsigned char tensionMinNb;					//Le numéro de la cellule à tension min
	unsigned char tensionMaxNb;					//Le numéro del a cellule à tension max        
	unsigned int tensionMin;					//Tension Min
	unsigned int tensionMax;					//Tension Max
	unsigned int temperatureMin;				//Temperature min
	unsigned int temperatureMax;				//Temperature max
	intM temperatureMaxResistance;				//Temperature du PCB de résistance
	intM cellBalancing;							//Les cellules que le balancement est actif
	Device deviceTable[NB_DEVICE_BY_BRANCH];	//Les BQ
}Branch;

Branch branch0;

unsigned char nbDevice;	//Nombre de device trouvé

//Mettre les valeurs initiales dans la branche
void initBranchData(Branch *branch, byte id);

//Assigner les addresse
byte assignAddress(Branch *branch);
//Mettre les valeurs par défaut dans les BQ
void setInitialConfig(Branch *branch);
//Lecture du status de chacun des devices
void status(Branch *branch);
//Reset du registre des alertes
void resetBranchAlert(Branch *branch);
//Reset du registre des Fautes
void resetBranchFault(Branch *branch);
//Met les bq d'un branche en mode sleep
void sleepBranch(Branch * branch);
//Réveille les bq d'un branche qui était en sleep
void wakeUpBranch(Branch *branch);
//Changer la valeurs du temps de conversion
void changeBranchConversionTime(Branch *branch, byte value);
//Mise à jour de la tension et de la température des cellules
void updateCellVoltage(Branch *branch, int bufferNB);
//Mise à jour des fet de chacune des cellules
unsigned char  updateFETs(Branch *branch);

unsigned int isCellBleeding(Branch *branch);


#endif //BRANCHE_H
