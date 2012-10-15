/*************************************************************************
 *
 *   
 *
 *    File name   :  device.h
 *    Description :  Méthode pour utilier le BQ
 *
 *    History :
 *    1. Date        : 	11-12-2011
 *       Author      : 	Joel Viau & Rudy Bernard
 *       Description : 
 *
 **************************************************************************/

#ifndef DEVICE_H
#define DEVICE_H

#include "bq_com.h"
#include "BQ_Branch.h"

//Configuration du temps de conversion
#define ADCT3US		0
#define ADCT6US		1
#define ADCT12US	2
#define ADCT24US	3


//Initialiser les valeurs dans la structure du device
void initDeviceData(Device *device);
//Initialiser les valeurs dans la structure de la cellule
void initCellData(Cell *cell);

//Assigner la configuration de défaut au BQ
void setDefaultConfig(byte branchId, byte device);
//Fait un reset du BQ
void resetDevice(byte branchId, byte device);
//Reset les alertes
void resetAlert(byte branchId, byte device);
//Reset les faultes
void resetFault(byte branchId, byte device);
//Changer le temps de conversion
void changeConversionTime(byte branchId, byte device, byte value); //Changer le temps de conversion Regarder les define ADCTX
//Met le BQ dans son mode de sleep
void sleepMode(byte branchId, byte device);
//Réveille le BQ du mode Sleep
void wakeUpDevice(byte branchId, byte device);
//Obtenir la tension et la température des cellules
void getCellVoltage(byte branchId,Device *device, int bufferNB,unsigned int *tensionMin,unsigned int *tensionMax, unsigned char *tensionMinNb, unsigned char *tensionMaxNb);
//Obtenir la température TS1
void getTemperature1(byte branchId,Device *device);
//Obtenir la température TS2
void getTemperature2(byte branchId,Device *device);
//Obtenir la valeur du GPAI
void getGPAI(byte branchId,Device *device);
//Configurer la base de temps du BQ
void configTimer(byte branchId, byte device);
//Met à jour les fet de chaque cellule selon leur tension
unsigned char  triggerFET(byte branchId, Device *device, unsigned int tensionMin);
//Met à jour les données concernant les fets pour savoir s'il bleed
void updateFetStatus(byte branchId, Device *device);

#endif //DEVICE_H
