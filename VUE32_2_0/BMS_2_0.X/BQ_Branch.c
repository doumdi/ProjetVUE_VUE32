/*************************************************************************
 *
 *   
 *
 *    File name   :  branch.c
 *    Description :  Méthodes utilisées pour controler une branche en série de BQ
 *
 *    History :
 *    1. Date        : 	11-12-2011
 *       Author      : 	Joel Viau
 *       Description : 
 *
 **************************************************************************/

#include "BQ_Branch.h"
//#include "delay.h"
//#include "error.h"

#define IO_CTRL_DEFAULT 0x03 //TS1 TS2 ON

//------------------------------------------------------------------------------
//Initialisation des données d'une branche
void initBranchData(Branch *branch, byte id)
{
	nbDevice = NB_DEVICE_BY_BRANCH;
	unsigned char n =0;
	branch->id = id;
	branch->tensionMin = 0;
	branch->tensionMax = 0;
	branch->tensionMinNb = 0;
	branch->tensionMaxNb = 0;
	branch->temperatureMin = 0;
	branch->temperatureMax = 0;

	branch->temperatureMaxResistance.value = 0;
	branch->temperatureMaxResistance.changed = 0;
	branch->cellBalancing.value = 0;
	branch->cellBalancing.changed = 0;
	for(n=0;n < nbDevice; n++)
	{
		initDeviceData(&branch->deviceTable[n]);
	}
}

//------------------------------------------------------------------------------
byte assignAddress(Branch *branch)
{
  byte nbDeviceFound = 0;
  byte i, n;
  byte device_status;
  byte address;

  i=NB_DEVICE_BY_BRANCH; //controls iteration loop
  while (i>0)
  {
    //*Send BROADCAST_RESET to address 0x00*/
    resetDevice(branch->id,0x3F);
    
    n=0;  //controls number of discovered devices
	nbDeviceFound=0;
    while (n<NB_DEVICE_BY_BRANCH)
    {
    //*Read DEVICE_STATUS reg at address 0x00*/
    readRegister(branch->id, 0x00,DEVICE_STATUS,1,&device_status);
  
      //*Verify if MSB is equal to 0*/
      if (device_status & (1<<7))
      {
           n = NB_DEVICE_BY_BRANCH; //break internal loop
      }
      else
      {
        //*Assign a new address*/        

        //Save assigned address
        n++;
        branch->deviceTable[n-1].address = n;
        
        //ADDR_CTRL = n;   
        writeRegister(branch->id,0x00,ADDRESS_CONTROL,n);
        
        //read ADDR_CTRL
        readRegister(branch->id, n, ADDRESS_CONTROL, 1,&address);
        if ((address&0x3F) == n)
        {
		  nbDeviceFound++;
		  //Fait flasher la led
		  writeRegister(branch->id,branch->deviceTable[n-1].address,IO_CONTROL,0x40);
		  delayTime(500);
		  writeRegister(branch->id,branch->deviceTable[n-1].address,IO_CONTROL,0x00);
          //address next device or finish device detection
        	if (n==NB_DEVICE_BY_BRANCH)
            	return n;
			//else
				//signalError(ERROR_BMS_NOT_FIND_ALL_DEVICE);
        }
        else
        {
          //break internal loop
          n = NB_DEVICE_BY_BRANCH;
        }
      }
    }

    i--;
	}
	nbDevice = nbDeviceFound;
	// Todo: signalError(ERROR_BMS_FAULT_TO_ASSIGN_ADDRESS);

        return 0;
}

//------------------------------------------------------------------------------
//Fait la configuration initiale des devices
void setInitialConfig(Branch *branch)
{
	setDefaultConfig(branch->id,BROADCAST_ADDRESS);
}

//------------------------------------------------------------------------------
//Mise à zero des faults
void resetBranchFault(Branch *branch)
{
	resetFault(branch->id,BROADCAST_ADDRESS);
}

//------------------------------------------------------------------------------
//Mise à zero des alerts
void resetBranchAlert(Branch *branch)
{
	resetAlert(branch->id,BROADCAST_ADDRESS);
}

//------------------------------------------------------------------------------
//Met a jour le status des registres pour les données du master
void status(Branch *branch)
{
	//byte data =0;
	int i =0;
	for(;i<nbDevice;i++)
	{	
		readRegister(branch->id,branch->deviceTable[i].address,DEVICE_STATUS,1,&branch->deviceTable[i].status);
		readRegister(branch->id,branch->deviceTable[i].address,ALERT_STATUS,1,&branch->deviceTable[i].alert);
		readRegister(branch->id,branch->deviceTable[i].address,FAULT_STATUS,1,&branch->deviceTable[i].fault);
		readRegister(branch->id,branch->deviceTable[i].address,COV_FAULT,1,&branch->deviceTable[i].cov_fault);
		readRegister(branch->id,branch->deviceTable[i].address,CUV_FAULT,1,&branch->deviceTable[i].cun_fault);
	}
}

//------------------------------------------------------------------------------
//Obtenir la tension des cellules
void updateCellVoltage(Branch *branch, int bufferNB)
{
    unsigned char i = 3;
    unsigned char deviceNb = 0;
    unsigned int tmpTensionMin =0;
    unsigned int tmpTensionMax =0;
    unsigned char tmpTensionMinNb = 0;
    unsigned char tmpTensionMaxNb = 0;
    unsigned int tmpTemperatureMaxRes = 0;
    unsigned char io_ctrl = 0;
    unsigned char tmpIO_ctrl = 0;

    //Met la tension sur la patte du Température 1 pour la conversion.
    readRegister(branch->id,branch->deviceTable[0].address,IO_CONTROL,1,&io_ctrl);
    tmpIO_ctrl = (io_ctrl | IO_CTRL_DEFAULT);
    writeRegister(branch->id,BROADCAST_ADDRESS,IO_CONTROL,tmpIO_ctrl);
    tmpIO_ctrl = 0;
    readRegister(branch->id,branch->deviceTable[0].address,IO_CONTROL,1,&tmpIO_ctrl);
    delayTime(100);

    //Envoie une commande de conversion
    writeRegister(branch->id,BROADCAST_ADDRESS,ADC_CONVERT,0x01);

    do{
            delayTime(100);
            //Regarde si la conversion est terminée
            readRegister(branch->id,branch->deviceTable[0].address,DEVICE_STATUS,1,&branch->deviceTable[0].status);
            if(i-- ==0)
            {
                    // Todo: signalError(ERROR_BMS_TEMPS_DE_CONVERSION_DEPASSE);
            }

    }while((branch->deviceTable[0].status & 0x01) == 0x00);

    for(deviceNb=0; deviceNb < nbDevice; deviceNb++)
    {
        //Lecture de la tension
        getCellVoltage(branch->id,&branch->deviceTable[deviceNb],bufferNB,&tmpTensionMin,&tmpTensionMax,&tmpTensionMinNb,&tmpTensionMaxNb);
        //Lecture de la température
        getTemperature1(branch->id,&branch->deviceTable[deviceNb]);
        getTemperature2(branch->id,&branch->deviceTable[deviceNb]);
        getGPAI(branch->id,&branch->deviceTable[deviceNb]);

        if(bufferNB == (BUFFER_MOY-1))
        {
            if(deviceNb ==0 || branch->tensionMin > tmpTensionMin)
            {
                    branch->tensionMin =  tmpTensionMin;
                    branch->tensionMinNb = deviceNb*NB_CELL_BY_DEVICE + tmpTensionMinNb;
            }
            if(deviceNb == 0 || branch->tensionMax < tmpTensionMax)
            {
                    branch->tensionMax = tmpTensionMax;
                    branch->tensionMaxNb = deviceNb*NB_CELL_BY_DEVICE + tmpTensionMaxNb;
            }
        }
        if(deviceNb ==0 || branch->temperatureMin > branch->deviceTable[deviceNb].temperature1.value)
        {
                branch->temperatureMin =  branch->deviceTable[deviceNb].temperature1.value;
        }
        if(deviceNb == 0 || branch->temperatureMax < branch->deviceTable[deviceNb].temperature1.value)
        {
                branch->temperatureMax = branch->deviceTable[deviceNb].temperature1.value;
        }
        //Temperature Resistance
        if(deviceNb == 0 || branch->temperatureMaxResistance.value < branch->deviceTable[deviceNb].temperature2.value)
        {
                tmpTemperatureMaxRes = branch->deviceTable[deviceNb].temperature2.value;
        }
        if(branch->temperatureMaxResistance.value < branch->deviceTable[deviceNb].temperature3.value)
        {
                tmpTemperatureMaxRes = branch->deviceTable[deviceNb].temperature3.value;
        }
    }

    if(tmpTemperatureMaxRes != branch->temperatureMaxResistance.value)
    {
            branch->temperatureMaxResistance.value = tmpTemperatureMaxRes;
            branch->temperatureMaxResistance.changed = 1;
    }
    //Enleve la tension sur la patte de temperature 1 pour économiser de l'énergie
    writeRegister(branch->id,BROADCAST_ADDRESS,IO_CONTROL,io_ctrl);
}

//------------------------------------------------------------------------------
unsigned char  updateFETs(Branch *branch)
{
	//unsigned char cellNb =0;
	unsigned char deviceNb = 0;
	//unsigned int tmpCellBalancing = 0;
	unsigned int balActivity = 0;
	
	for(deviceNb=0; deviceNb < nbDevice; deviceNb++)
	{
		balActivity = triggerFET(branch->id, &branch->deviceTable[deviceNb],branch->tensionMin);
		//Met dans une variable si les cellules bleed.
//		tmpCellBalancing = tmpCellBalancing | (branch->deviceTable[deviceNb].fetStatus << NB_CELL_BY_DEVICE * deviceNb);
	}
/*	
	if(branch->cellBalancing.value != tmpCellBalancing)
	{
		branch->cellBalancing.value = tmpCellBalancing;
		branch->cellBalancing.changed = 1;
	}
*/
	return balActivity;
}

unsigned int isCellBleeding(Branch *branch)
{
    unsigned char deviceNb = 0;
    unsigned int tmpCellBalancing = 0;


    for(deviceNb=0; deviceNb < nbDevice; deviceNb++)
    {
        updateFetStatus(branch->id, &branch->deviceTable[deviceNb]);
		//Met dans une variable si les cellules bleed.
		tmpCellBalancing = tmpCellBalancing | (branch->deviceTable[deviceNb].fetStatus << NB_CELL_BY_DEVICE * deviceNb);
    }

    if(branch->cellBalancing.value != tmpCellBalancing)
    {
		branch->cellBalancing.value = tmpCellBalancing;
		branch->cellBalancing.changed = 1;
    }
    return tmpCellBalancing;
}

//------------------------------------------------------------------------------
void sleepBranch(Branch *branch)
{
	writeRegister(branch->id,BROADCAST_ADDRESS,IO_CONTROL,0x40);
	unsigned char deviceNb = nbDevice-1;
	for(; deviceNb >= 0; deviceNb--)
	{
		delayTime(250);
		writeRegister(branch->id,branch->deviceTable[deviceNb].address,IO_CONTROL,0x00);
		sleepMode(branch->id,branch->deviceTable[deviceNb].address);
	}
}

//------------------------------------------------------------------------------
void wakeUpBranch(Branch *branch)
{
	unsigned char deviceNb = 0;
	for(deviceNb=0; deviceNb < nbDevice; deviceNb++)
	{
		delayTime(250);
		wakeUpDevice(branch->id, branch->deviceTable[deviceNb].address);
		writeRegister(branch->id,branch->deviceTable[deviceNb].address,IO_CONTROL,0x40);
	}
	writeRegister(branch->id,BROADCAST_ADDRESS,IO_CONTROL,0x00);
}

//------------------------------------------------------------------------------
void changeBranchConversionTime(Branch *branch,byte value)
{	
	unsigned char deviceNb = 0;
	
	for(deviceNb=0; deviceNb < nbDevice; deviceNb++)
	{
		changeConversionTime(branch->id, branch->deviceTable[deviceNb].address,value);
	}
}
