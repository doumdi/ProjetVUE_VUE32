/******************************************************************************
 * VUE32 #0 functions implementation
 * THIS VERSION IS USED AS A COMPLETE SIMULATOR FOR TESTING PURPOSE
 *
 * by Maxime Bedard - 24/09/2012
 * ****************************************************************************/

#include "NETV32_Common.h"
#include "HardwareProfile.h"
#include "VUE32_Utils.h"
#include "BQ_Branch.h"
#include "Board.h"
#include "BMS.h"

#define NB_ROUND_BEFORE_SEND 2
#define NB_ROUND_BEFORE_SEND_STATUS 10

// Local variables
unsigned int statusCmp;
unsigned int bufferMoy;
enum eStates m_state, lastState;
int sendData;	//Compteur pour le delay pour envoyer sur le CAN

// Prototypes
void balance(void);
void monitor(void);
void openContactor(void);
void openCloseAllIO(void);

void delayTime(unsigned int time) //0 à 536 000ms
{
    // Todo: STUB! Remove this function!
    while(time) time--;
}

void setState(unsigned char etat)
{
    lastState = m_state;
    m_state = etat;
}

/*
 * State Machine Processing
 */
void ImplBMS(void)
{
    //------------------------------------
    // Gestion du fonctionnement de la machine a État
    switch (m_state) {
        case InitPeripheral:
        {
            statusCmp = 0;
            bufferMoy = 0;
            setState(InitBQ);
            break;
        }
        case InitBQ:
        {
            //initBQ();
            initBranchData(&branch0, 0); // Met les valeurs à zéro
            assignAddress(&branch0); // Assigne les address au device
            status(&branch0); // Mise à jour des registres de status des devices
            setInitialConfig(&branch0); // Assigne la configuration initiale
            resetBranchFault(&branch0); // Reset des fautes
            resetBranchAlert(&branch0); // Reset des alertes
            status(&branch0); // Mise à jour des status

            setState(Monitor);
            break;
        }
        case Monitor:
        {
            EVERY_X_MS(2000)
                monitor();
            END_OF_EVERY
            break;
        }
        case Balance:
        {
            EVERY_X_MS(500)
                balance();
            END_OF_EVERY
            break;
        }
        case InitSleep:
        {
            sleepBranch(&branch0); //Met les BQ dans le mode sleep
            setState(Sleep);
            break;
        }
        case Sleep:
        {
            break;
        }
        case WakeUp:
        {
            wakeUpBranch(&branch0); //Réinitialise les BQ après le mode sleep
            setState(InitBQ);
            break;
        }
        case WaitStabiliseTension:
        {
            if (waitStabiliseTension() == 1)
                setState(Balance);
            break;
        }
        case ProblemDetected:
        {
            if (problemDetected() == 1)
                setState(Monitor);
            break;
        }
        case WaitStabiliseTemp:
        {
            if (waitStabiliseTemp() == 1)
                setState(Balance);
            break;
        }
        case Test:
        {
            openCloseAllIO();
            break;
        }
        default:
            break;
    }
    bufferMoy++;
}

/*
 * Message Processing
 */
void OnMsgBMS(NETV_MESSAGE *msg)
{
    unsigned char ucTest;
    unsigned short usTest;
    unsigned int unTest;

    // On every message received, toggle the LED2
    LED2 = ~LED2;

    // Deal with SETVALUE requests
    ON_MSG_TYPE( VUE32_TYPE_SETVALUE )
        ACTION3(E_ID_LOWBEAM, unsigned char, ucTest, unsigned char, ucTest, unsigned short, usTest)
            unTest = ((unsigned int)ucTest << 16) + usTest;
            ANSWER3(E_ID_LOWBEAM, unsigned int, unTest, unsigned short, usTest, unsigned char, ucTest)
        END_OF_ACTION
    END_OF_MSG_TYPE
}

//--------------------------------------------------------------------------------------------------------------

void balance(void) {
    status(&branch0); // Mise à jour des status

    if(isCellBleeding(&branch0) == 0)
    {
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise à jour des tensionn et de la température

        if (verifyTensionMax() || verifyTemperatureCell()) {
            openContactor();
            setState(WaitStabiliseTension);
            return;
        }
        if (verifyTemperatureRes()) {
            openContactor();
            setState(WaitStabiliseTemp);
            return;
        }


        updateFETs(&branch0); // Mise à jour des FETs pour le balancement

    }
    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress); //Envoie les données de tensions et température sur le CAN
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les données de satus sur le CAN
}

//--------------------------------------------------------------------------------------------------------------

void monitor(void) {
    status(&branch0); // Mise à jour des status
    updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise à jour des tensions et de la température

    if(branch0.cellBalancing.value != 0)
    {
            branch0.cellBalancing.value = 0;
	    branch0.cellBalancing.changed = 1;
    }

    if (verifyTensionMin() || verifyTemperatureCell())
    {
        openContactor();
        setState(ProblemDetected);
    }

    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress); //Envoie les données de tensions et température sur le CAN
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les données de satus sur le CAN

}

//--------------------------------------------------------------------------------------------------------------

void openContactor(void) {
    CANTransmetOpenContactor(); //Ouvre les contacteurs
}

int verifyTensionMax() {

    if (branch0.tensionMax > tensionMaxOpenContactor) {
        //Debounce
        delayTime(500);
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY);
        if (branch0.tensionMax > tensionMaxOpenContactor)
            return 1;

    }
    return 0;
}

int verifyTensionMin() {
    //Si la tension est sous la tension minimale ou au dessu de la tension maximale des cellules
    if (branch0.tensionMin < tensionMinCell) {
        //Debounce
        delayTime(500);
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY);
        if (branch0.tensionMin < tensionMinCell)
            return 1;
    }
    return 0;
}

int verifyTemperatureCell() {
    //Si la température est au dessus de la température maximale des cellules
    if (branch0.temperatureMax >= temperatureMaxCell) {
        //Debounce
        delayTime(500);
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // TODO Update juste Temperature
        if (branch0.temperatureMax >= temperatureMaxCell)
            return 1;
    }
    return 0;
}

int verifyTemperatureRes() {
    if (branch0.temperatureMaxResistance.value >= temperatureMaxRes) {
        //Debounce
        delayTime(500);
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // TODO Update juste Temperature
        if (branch0.temperatureMaxResistance.value >= temperatureMaxRes)
            return 1;
    }
    return 0;
}

int waitStabiliseTension() {
    status(&branch0); // Mise à jour du status

    if(isCellBleeding(&branch0) == 0)
    {
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise à jour des tensions et de la température

        if (verifyTemperatureRes()) {
            setState(WaitStabiliseTemp);
            return 0;
        }
        updateFETs(&branch0); // Mise à jour des fets
    }
    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress);
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les données de satus sur le CAN

    //Si la tension max est de retour à la tension ajusté, on change d'état pour retourner à l'état précédent
    if (branch0.tensionMax <= (tensionMaxOpenContactor - 50) ) {
        return 1;
    }
    return 0;
}

int problemDetected()
{
    status(&branch0); // Mise à jour du status
    updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise à jour des tensions et de la température

    if(branch0.cellBalancing.value != 0)
    {
            branch0.cellBalancing.value = 0;
	    branch0.cellBalancing.changed = 1;
    }

    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress);
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les données de satus sur le CAN

    //Si la tension max est de retour à la tension ajusté, on change d'état pour retourner à balance
    if (!verifyTensionMin() && !verifyTemperatureCell()) {
        return 1;
    }
    return 0;
}

int waitStabiliseTemp() {
    status(&branch0); // Mise à jour du status
    updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise à jour des tensions et de la température

    if(branch0.cellBalancing.value != 0)
    {
            branch0.cellBalancing.value = 0;
	    branch0.cellBalancing.changed = 1;
    }

    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress);
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les données de satus sur le CAN

    //Si la tension max est de retour à la tension ajusté, on change d'état pour retourner à balance
    if (branch0.temperatureMaxResistance.value <= temperatureMaxRes - 50) {
        return 1;
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------------------

void openCloseAllIO(void) {
    writeRegister(branch0.id, BROADCAST_ADDRESS, IO_CONTROL, 0x40); //Open all LED
    writeRegister(branch0.id, BROADCAST_ADDRESS, CB_CTRL, 0x1F); //Open all FET
    //toggleLed1();
    //toggleLed2();
    delayTime(100);

    writeRegister(branch0.id, BROADCAST_ADDRESS, IO_CONTROL, 0x00); //Close all LED
    writeRegister(branch0.id, BROADCAST_ADDRESS, CB_CTRL, 0x00); //Close all FET
    //toggleLed1();
    //toggleLed2();

    delayTime(100);

}

