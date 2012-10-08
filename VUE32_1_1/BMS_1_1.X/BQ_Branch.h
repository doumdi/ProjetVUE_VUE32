/*************************************************************************
 *
 *   
 *
 *    File name   :  branch.h
 *    Description :  M�thodes utilis�es pour controler une branche en s�rie de BQ
 *
 *    History :
 *    1. Date        : 	11-12-2011
 *       Author      : 	Joel Viau Le meilleur
 *       Description : 
 *
 **************************************************************************/

#ifndef BRANCHE_H
#define BRANCHE_H

#include "BQ.h"
#include "BQ_com.h"



//Data for branch
typedef struct {
	unsigned char id;							//Id de la branche
	unsigned int tensionMin;					//Tension Min
	unsigned int tensionMax;					//Tension Max
	unsigned char tensionMinNb;					//Le num�ro de la cellule � tension min
	unsigned char tensionMaxNb;					//Le num�ro del a cellule � tension max
	unsigned int temperatureMin;				//Temperature min
	unsigned int temperatureMax;				//Temperature max
	intM temperatureMaxResistance;				//Temperature du PCB de r�sistance
	intM cellBalancing;							//Les cellules que le balancement est actif
	Device deviceTable[NB_DEVICE_BY_BRANCH];	//Les BQ
}Branch;

Branch branch0;

unsigned char nbDevice;	//Nombre de device trouv�

//Mettre les valeurs initiales dans la branche
void initBranchData(Branch *branch, byte id);

//Assigner les addresse
byte assignAddress(Branch *branch);
//Mettre les valeurs par d�faut dans les BQ
void setInitialConfig(Branch *branch);
//Lecture du status de chacun des devices
void status(Branch *branch);
//Reset du registre des alertes
void resetBranchAlert(Branch *branch);
//Reset du registre des Fautes
void resetBranchFault(Branch *branch);
//Met les bq d'un branche en mode sleep
void sleepBranch(Branch * branch);
//R�veille les bq d'un branche qui �tait en sleep
void wakeUpBranch(Branch *branch);
//Changer la valeurs du temps de conversion
void changeBranchConversionTime(Branch *branch, byte value);
//Mise � jour de la tension et de la temp�rature des cellules
void updateCellVoltage(Branch *branch, int bufferNB);
//Mise � jour des fet de chacune des cellules
unsigned char  updateFETs(Branch *branch);

unsigned int isCellBleeding(Branch *branch);


#endif //BRANCHE_H