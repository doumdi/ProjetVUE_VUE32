/*************************************************************************
 *
 *   
 *
 *    File name   :  device.c
 *    Description :  Méthode pour utilier le BQ
 *
 *    History :
 *    1. Date        : 	11-12-2011
 *       Author      : 	Joel Viau & Rudy Bernard
 *       Description : 
 *
 **************************************************************************/
#include "BQ.h"
#include "BMS.h"
//#include "error.h"
//#include "delay.h"


#define BALANCING_RESISTOR			8				// omh of the balancing resistance
#define BALANCING_TIME				2				// time in seconds of activation of the balancing mosfet
#define BALANCINT_IN_MIN			0x80			// time scale in minute
#define BALANCINT_IN_SEC			0x00			// time scale in second





#define DEFAULT_FUNCTION_CONFIG 	0x28			// Met le temps de conversion à 4us et le GPAI_REF à VReg50
#define DEFAULT_IO_CONFIG			0x00			// 
#define DEFAULT_OVERVOLTAGE 		0x24			// OverVoltage = 3,6V
#define DEFAULT_OVERVOLTAGETIME 	0x0A			// 1000useconde
#define DEFAULT_UNDERVOLTAGE 		0x14			// underVoltage = 2,0
#define DEFAULT_UNDERVOLTAGETIME 	0x0A			// 1000useconde
#define DEFAULT_OVERTEMP 		0x00			// Désactivé pour le moment
#define DEFAULT_OVERTEMPTIME 		0x00

#define DEFAULT_ADC			0x78			// ADC_ON et TS1 TS2 GPAI
//#define DEFAULT_CBTIME			0x3C			// 60sec pour l'activation des FET

void initDeviceData(Device *device) {
    unsigned char j = 0;
    device->status = 0;
    device->alert = 0;
    device->fault = 0;
    device->fetStatus = 0;
    device->address = 0;
    device->cov_fault = 0;
    device->cun_fault = 0;
    device->temperature1.value = 0;
    device->temperature1.changed = 0;
    device->temperature2.value = 0;
    device->temperature2.changed = 0; //1 si la température à changé
    device->temperature3.value = 0;
    device->temperature3.changed = 0;
    for (j = 0; j < NB_CELL_BY_DEVICE; j++) {
        initCellData(&device->cellTable[j]);
    }
}

void initCellData(Cell *cell) {
    int i = 0;
    cell->tension.value = 0;
    cell->tension.changed = 0;
    cell->charge.value = 0;
    cell->charge.changed = 0;
    cell->balanceActive = 0;
    for (i = 0; i < BUFFER_MOY; i++)
        cell->bufferTension[i] = 0;
}

//------------------------------------------------------------------------------

void setDefaultConfig(byte branchId, byte device) {
    byte adcCtrl = 0;
    writeRegister(branchId, device, SHDW_CTRL, 0x35);
    writeRegister(branchId, device, FUNCTION_CONFIG, DEFAULT_FUNCTION_CONFIG);
    writeRegister(branchId, device, SHDW_CTRL, 0x35);
    writeRegister(branchId, device, IO_CONFIG, DEFAULT_IO_CONFIG);
    writeRegister(branchId, device, SHDW_CTRL, 0x35);
    writeRegister(branchId, device, CONFIG_COV, DEFAULT_OVERVOLTAGE);
    writeRegister(branchId, device, SHDW_CTRL, 0x35);
    writeRegister(branchId, device, CONFIG_COVT, DEFAULT_OVERVOLTAGETIME);
    writeRegister(branchId, device, SHDW_CTRL, 0x35);
    writeRegister(branchId, device, CONFIG_CUV, DEFAULT_UNDERVOLTAGE);
    writeRegister(branchId, device, SHDW_CTRL, 0x35);
    writeRegister(branchId, device, CONFIG_CUVT, DEFAULT_UNDERVOLTAGETIME);
    writeRegister(branchId, device, SHDW_CTRL, 0x35);
    writeRegister(branchId, device, CONFIG_OT, DEFAULT_OVERTEMP);
    writeRegister(branchId, device, SHDW_CTRL, 0x35);
    writeRegister(branchId, device, CONFIG_OTT, DEFAULT_OVERTEMPTIME);

    writeRegister(branchId, device, CB_TIME, BALANCING_TIME | BALANCINT_IN_SEC);

    adcCtrl = DEFAULT_ADC | (NB_CELL_BY_DEVICE - 1);
    writeRegister(branchId, device, ADC_CONTROL, adcCtrl);

}
//------------------------------------------------------------------------------

void resetDevice(byte branchId, byte device) {
    writeRegister(branchId, device, RESET, 0xA5);
}
//------------------------------------------------------------------------------

void resetAlert(byte branchId, byte device) {
    byte data = 0xEF;
    writeRegister(branchId, device, ALERT_STATUS, data);
    data = 0x6F;
    writeRegister(branchId, device, ALERT_STATUS, data);

}

//------------------------------------------------------------------------------

void resetFault(byte branchId, byte device) {
    byte data = 0;
    readRegister(branchId, device, FAULT_STATUS, 0x01, &data);
    data = 0xEF;
    writeRegister(branchId, device, FAULT_STATUS, data);
    data = 0;
    writeRegister(branchId, device, FAULT_STATUS, data);
}

//------------------------------------------------------------------------------

void changeConversionTime(byte branchId, byte device, byte value) {
    if (device != 0) {
        byte data = 0;
        readRegister(branchId, device, FUNCTION_CONFIG, 0x01, &data);
        data = (data & 0x3F) | (value << 6);
        writeRegister(branchId, device, FUNCTION_CONFIG, data);
    }
}

//------------------------------------------------------------------------------

void sleepMode(byte branchId, byte device) {
    if (device != 0) {
        byte data = 0;
        readRegister(branchId, device, IO_CONTROL, 0x01, &data);
        data |= 0x04;
        writeRegister(branchId, device, IO_CONTROL, data);
        delayTime(100000);

        readRegister(branchId, device, ALERT_STATUS, 0x01, &data);
        data &= 0xFB;
        writeRegister(branchId, device, ALERT_STATUS, data);
        data = 0;
    }
}

//------------------------------------------------------------------------------

void wakeUpDevice(byte branchId, byte device) {
    if (device != 0) {
        byte data = 0;
        readRegister(branchId, device, IO_CONTROL, 0x01, &data);
        data &= 0xFB;
        writeRegister(branchId, device, IO_CONTROL, data);
        delayTime(10);
    }
}

//------------------------------------------------------------------------------

void getCellVoltage(byte branchId, Device *device, int bufferNB, unsigned int *tensionMin, unsigned int *tensionMax, unsigned char *tensionMinNb, unsigned char *tensionMaxNb) {
    unsigned char cellNb = 0;
    byte tmpVCELL[2] = {0};
    byte registerNb = VCELL1;
    unsigned long tmp = 0;

    for (cellNb = 0; cellNb < NB_CELL_BY_DEVICE; cellNb++) {
        //reading and formating of the 14 bits of the ADC value
        readRegister(branchId, device->address, registerNb, 2, tmpVCELL);
        /*
        {//signalError(ERROR_BMS_CRC);
        }
         */

        tmp = tmpVCELL[1] * 0x100 + tmpVCELL[0];

        //convert into voltage
        tmp = tmp * 6250 / 16383;


        device->cellTable[cellNb].bufferTension[bufferNB] = tmp;


        tmp = 0;
        int i = 0;
        for (i = 0; i < BUFFER_MOY; i++)
            tmp = tmp + device->cellTable[cellNb].bufferTension[bufferNB];
        tmp = tmp / BUFFER_MOY;

        if (device->cellTable[cellNb].tension.value != tmp) {
            device->cellTable[cellNb].tension.changed = 1;
            device->cellTable[cellNb].tension.value = tmp;
        }

        if (cellNb == 0 || (*tensionMin) > tmp) {
            (*tensionMin) = tmp;
            (*tensionMinNb = cellNb);
        }
        if (cellNb == 0 || (*tensionMax) < tmp) {
            (*tensionMax) = tmp;
            (*tensionMaxNb = cellNb);
        }

        //increase address register for the next cell
        registerNb = registerNb + 0x02;
    }
}

//------------------------------------------------------------------------------

void getTemperature1(byte branchId, Device *device) {
    byte tmpTEMP[2] = {0};
    unsigned long tmp = 0;
    byte registerNb = TEMPERATURE1;
    readRegister(branchId, device->address, registerNb, 2, tmpTEMP);
    tmp = tmpTEMP[1] * 0x100 + tmpTEMP[0];
    tmp = tmp / 20;
    //tmp = tmp * 33104 / 6420;
    if (device->temperature1.value != tmp) {
        device->temperature1.changed = 1;
        device->temperature1.value = tmp;
    }

}
//------------------------------------------------------------------------------

void getTemperature2(byte branchId, Device *device) {
    byte tmpTEMP[2] = {0};
    unsigned long tmp = 0;
    byte registerNb = TEMPERATURE2;
    readRegister(branchId, device->address, registerNb, 2, tmpTEMP);
    tmp = tmpTEMP[1] * 0x100 + tmpTEMP[0];
    tmp = tmp / 13;
    //tmp = tmp * 33104 / 6420;
    if (device->temperature2.value != tmp) {
        device->temperature2.changed = 1;
        device->temperature2.value = tmp;
    }

}
//------------------------------------------------------------------------------

void getGPAI(byte branchId, Device *device) {
    byte tmpTEMP[2] = {0};
    unsigned long tmp = 0;
    byte registerNb = GPAI;
    readRegister(branchId, device->address, registerNb, 2, tmpTEMP);
    tmp = tmpTEMP[1] * 0x100 + tmpTEMP[0];
    tmp = tmp / 13;
    //tmp = tmp * 33104 / 6420;
    if (device->temperature3.value != tmp) {
        device->temperature3.changed = 1;
        device->temperature3.value = tmp;
    }

}
//------------------------------------------------------------------------------

void configTimer(byte branchId, byte device) {
    writeRegister(branchId, device, CB_TIME, BALANCING_TIME | BALANCINT_IN_SEC);
}

//------------------------------------------------------------------------------

unsigned char triggerFET(byte branchId, Device *device, unsigned int tensionMin) {
    unsigned char cellNb = 0;
    byte cb_ctrl = 0;
	unsigned char balActivity = 0;

    //Read the state of the balancing FET
    readRegister(branchId, device->address, CB_CTRL, 1, &device->fetStatus);

    for (cellNb = 0; cellNb < NB_CELL_BY_DEVICE; cellNb++) {
        //If the cell voltage is greater than the maximum tolerated voltage
        if (device->cellTable[cellNb].tension.value > tensionMaxCell) {
            //If the FET is not already activated
            device->cellTable[cellNb].balanceActive = 1;
            cb_ctrl = cb_ctrl | (0x01 << cellNb);
			balActivity++;
        }
        else if (device->fMaxTensionForced && device->cellTable[cellNb].tension.value > (device->nMaxTensionForced + gapOverMinCell)  && device->cellTable[cellNb].tension.value > tensionStartFet)
        {
            //If the FET is not already activated
            device->cellTable[cellNb].balanceActive = 1;
            cb_ctrl = cb_ctrl | (0x01 << cellNb);
			balActivity++;
        }
        //Si la tension est plus grande que le plus petite tension de la branche plus le gap
        else if (!device->fMaxTensionForced && device->cellTable[cellNb].tension.value > (tensionMin + gapOverMinCell) && device->cellTable[cellNb].tension.value > tensionStartFet) {
            device->cellTable[cellNb].balanceActive = 1;
			balActivity++;
            cb_ctrl = cb_ctrl | (0x01 << cellNb);
        } 
	else {
            device->cellTable[cellNb].balanceActive = 0;
        }
    }

    device->fetStatus = cb_ctrl;
    writeRegister(branchId, device->address, CB_CTRL, device->fetStatus);

	return balActivity;
}

//------------------------------------------------------------------------------

int coulombBalancingLost(int vcell) {
    return vcell / BALANCING_RESISTOR * BALANCING_TIME / 1000;
}
void updateFetStatus(byte branchId, Device *device)
{
    readRegister(branchId, device->address, CB_CTRL, 1, &device->fetStatus);
}

//------------------------------------------------------------------------------
