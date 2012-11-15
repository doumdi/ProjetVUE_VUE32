/******************************************************************************
 * VUE32 #0 functions implementation
 * THIS VERSION IS USED AS A COMPLETE SIMULATOR FOR TESTING PURPOSE
 *
 * by Maxime Bedard - 24/09/2012
 * ****************************************************************************/

#include "../src/NETV32_Common.h"
#include "HardwareProfile.h"
#include "../src/VUE32_Utils.h"
#include "BQ_Branch.h"
#include "Board.h"
#include "BMS.h"
#include "BMS_Impl.h"

void initialiseIO();

HDW_MAPPING gBMS_Ress[] =
{
    {E_ID_BMS_BOARD_TEMP, 2, Sensor},
    {E_ID_BMS_CELL_GROUP1, 7, Sensor},
    {E_ID_BMS_CELL_GROUP2, 7, Sensor},
};

#define NB_ROUND_BEFORE_SEND 2
#define NB_ROUND_BEFORE_SEND_STATUS 10

// Local variables
unsigned int statusCmp;
unsigned int bufferMoy;
//enum eStates m_state, lastState;
int sendData;	//Compteur pour le delay pour envoyer sur le CAN

// From CANFunctions.c
extern BMSAddress bmsAddress;
unsigned char changeTensionReceive = 0; //Permet d'envoyer un Acknoledge quand les 2 parties du messages sont recu

// Prototypes
void ImplBMS(void);
void balance(void);
void monitor(void);
void openContactor(void);
void openCloseAllIO(void);
int waitStabiliseTension();
int problemDetected();
void saveDataToMemory();
int verifyTensionMin();
int verifyTemperatureCell();
int verifyTensionMax();
int verifyTemperatureRes();
int waitStabiliseTemp();
void InitializeSPI();
void netv_init_can_driver(unsigned char canAddr, CAN_MODULE CANx);
unsigned char getAddress();
unsigned char getBranchAddress();

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
            initializePeripheral();
            setState(InitBQ);
            break;
        }
        case InitBQ:
        {
            initBranchData(&branch0, 0); // Met les valeurs à zéro
            assignAddress(&branch0); // Assigne les address au device
            status(&branch0); // Mise à jour des registres de status des devices
            setInitialConfig(&branch0); // Assigne la configuration initiale
            resetBranchFault(&branch0); // Reset des fautes
            resetBranchAlert(&branch0); // Reset des alertes
            status(&branch0); // Mise à jour des status

            /*LP_PARAMS newLongPolling;
            HDW_MAPPING *gVUE32_Ress;

            gVUE32_Ress = gHardwareMapBMS[INDEX_BMS];

            newLongPolling.ucSizeData = gVUE32_Ress[i].ucDataSize
            newLongPolling.hasLifeTime = LONG_POLLING_WITHOUT_LIFE_TIME;
            newLongPolling.ucDestinataire = 0;
            newLongPolling.ucResourceId = E_ID_BMS_BOARD_TEMP;
            newLongPolling.unDelay = 100;
            newLongPolling.ucMsgType = VUE32_TYPE_SETVALUE;*/
            
            setState(Monitor);
            
            //Enables the core to handle any pending interrupt requests
            asm volatile ("ei");

            break;
        }
        case Monitor:
        {
            EVERY_X_MS(2000)
                    LED1 ^= 1;
                    monitor();
            END_OF_EVERY
            setState(InitSleep);
            break;
        }
        case Balance:
        {
            EVERY_X_MS(500)
                LED1 ^= 1;
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
            setState(WakeUp);
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

    //if ( m_state == ProblemDetected)
    //{
    EVERY_X_MS(2000)
        unsigned char io_ctrl;
        readRegister(branch0.id,branch0.deviceTable[0].address,IO_CONTROL,1,&io_ctrl);
        if ( io_ctrl & 0x40 )
            io_ctrl = (io_ctrl & 0xBF);
        else
            io_ctrl = (io_ctrl | 0x40);
        writeRegister(branch0.id,BROADCAST_ADDRESS,IO_CONTROL,io_ctrl);
    END_OF_EVERY
    //}

    //If the ErrorStateFlag is set
    if(ErrorStateFlag == TRUE)
    {
        setState(Monitor);

        resetBranchFault(&branch0); // Reset des fautes
        resetBranchAlert(&branch0); // Reset des alertes

        ErrorStateFlag = FALSE;
    }

    bufferMoy++;
}

/*
 * Message Processing
 */
void OnMsgBMS(NETV_MESSAGE *msg)
{
    char cmd = 0;
    cmd = msg->msg_cmd;

    // ********* Deal with SETVALUE requests **********
    ON_MSG_TYPE(VUE32_TYPE_SETVALUE)
            
            // Error Flag
            ACTION1(E_ID_BMS_STATE_ERROR, unsigned char, ErrorStateFlag)
                ErrorStateFlag = TRUE; // No matter what the payload is, this cmd always activate the error flag
            END_OF_ACTION
            
            // Four tensions of the BMS
            ACTION4(E_ID_BMS_TENSIONS, short, tensionMaxCell, short, gapOverMinCell, short, tensionStartFet, short, tensionMinCell)
            END_OF_ACTION
            
            // Three BMS Temperatures
            ACTION3(E_ID_BMS_TEMPERATURES, short, temperatureMaxCell, short, temperatureMaxRes, short, tensionMaxOpenContactor)
                branch0.temperatureMaxResistance.changed = 1; // Flag to notify userspace of the change
            END_OF_ACTION
                    
            // Set the balancing state of the BMS (on/off), this command also allows us to reset BQs
            unsigned char fBalancing = 0, fForced = 0;
            unsigned short sForcedTension;
            ACTION3(E_ID_BMS_STATE_BALANCING, unsigned char, fBalancing, unsigned char, fForced, unsigned short, sForcedTension)
                if (fForced == 1)
                {
                    branch0.deviceTable[0].fMaxTensionForced = 1;
                    branch0.deviceTable[0].nMaxTensionForced = (unsigned int)sForcedTension;
                    branch0.deviceTable[1].fMaxTensionForced = 1;
                    branch0.deviceTable[1].nMaxTensionForced = (unsigned int)sForcedTension;
                }
                else
                {
                    branch0.deviceTable[0].fMaxTensionForced = 0;
                    branch0.deviceTable[0].nMaxTensionForced = (unsigned int)branch0.tensionMin;
                    branch0.deviceTable[1].fMaxTensionForced = 0;
                    branch0.deviceTable[1].nMaxTensionForced = (unsigned int)branch0.tensionMin;
                }
                setState( (fBalancing == 1) ? Balance : InitBQ );
            END_OF_ACTION

            // temp
            unsigned short usTempState;
            ACTION1(0x1E, unsigned short, usTempState)
                m_state = usTempState;
                branch0.deviceTable[0].fMaxTensionForced = 0;
                branch0.deviceTable[0].nMaxTensionForced = 0;
                branch0.deviceTable[1].fMaxTensionForced = 0;
                branch0.deviceTable[1].nMaxTensionForced = 0;
            END_OF_ACTION
                    
    END_OF_MSG_TYPE
            
    // ********* Deal with GETVALUE requests **********
    ON_MSG_TYPE(VUE32_TYPE_GETVALUE)

            // Maximum Temperature in the resistor array
            ANSWER1(E_ID_BMS_BOARD_TEMP, short, (short)branch0.temperatureMaxResistance.value)                    

            // temp
            if (m_state == 3)
            {
                ANSWER3(E_ID_BMS_STATE_READONLY, short, (short)m_state, short, (short) branch0.deviceTable[0].fMaxTensionForced, unsigned short, (unsigned short)branch0.deviceTable[0].nMaxTensionForced )
            }
            else
            {
                ANSWER1(E_ID_BMS_STATE_READONLY, short, (short)m_state)
            }

            if ( msg->msg_cmd == E_ID_BMS_CELL_GROUP1)
            {
                NETV_MESSAGE oCopy = *msg;
                ANSWER4(E_ID_BMS_CELL_GROUP1, short, CG_ID_BMS_CELLGROUP_VOLT1_TEMP,
                        short, (short)branch0.deviceTable[0].cellTable[0].tension.value,
                        short, (short)branch0.deviceTable[0].cellTable[1].tension.value,
                        short, (short)branch0.deviceTable[0].temperature1.value)

                *msg = oCopy; // Reset the msg structure to allow proper interpretation
                ANSWER3(E_ID_BMS_CELL_GROUP1, short, CG_ID_BMS_CELLGROUP_VOLT2,
                        short, (short)branch0.deviceTable[0].cellTable[2].tension.value,
                        short, (short)branch0.deviceTable[0].cellTable[3].tension.value)
            }

            if ( msg->msg_cmd == E_ID_BMS_CELL_GROUP2)
            {
                NETV_MESSAGE oCopy = *msg;
                ANSWER4(E_ID_BMS_CELL_GROUP2, short, CG_ID_BMS_CELLGROUP_VOLT1_TEMP,
                        short, (short)branch0.deviceTable[1].cellTable[0].tension.value,
                        short, (short)branch0.deviceTable[1].cellTable[1].tension.value,
                        short, (short)branch0.deviceTable[1].temperature1.value)

                *msg = oCopy; // Reset the msg structure to allow proper interpretation
                ANSWER3(E_ID_BMS_CELL_GROUP2, short, CG_ID_BMS_CELLGROUP_VOLT2,
                        short, (short)branch0.deviceTable[1].cellTable[2].tension.value,
                        short, (short)branch0.deviceTable[1].cellTable[3].tension.value)
            }
    END_OF_MSG_TYPE

    // ********* Deal with STARTEMETTING requests **********
    ON_MSG_TYPE(VUE32_TYPE_STARTEMETTING)
            ActionStartEmettings(msg);
    END_OF_MSG_TYPE

    // ********* Deal with STOPEMETTING requests **********
    ON_MSG_TYPE(VUE32_TYPE_STOPEMETTING)
            DesactivateLongPolling(msg->msg_cmd);
    END_OF_MSG_TYPE
}

//--------------------------------------------------------------------------------------------------------------
//CAN transmission functions
void CANTransmetErrorState()
{
    // TODO: A corrigwer
    NETV_MESSAGE msg;

    msg.msg_priority = NETV_PRIORITY_MEDIUM;
    msg.msg_type = VUE32_TYPE_GETVALUE;
    msg.msg_cmd = E_ID_BMS_STATE_ERROR;
    msg.msg_dest = ADDR_CAN_ODB;
    msg.msg_source = GetMyAddr();
    msg.msg_comm_iface = NETV_COMM_IFACE_CAN1;
    msg.msg_remote = 0;
    msg.msg_data_length = 1;    
    msg.msg_data[0] = m_state;

    netv_send_message(&msg);
}

void CANTransmetOpenContactor(void) {

    NETV_MESSAGE msg;

    msg.msg_priority = NETV_PRIORITY_HIGHEST;
    msg.msg_type = NETV_TYPE_EMERGENCY;
    msg.msg_cmd = E_ID_MAIN_CONTACTOR;
    msg.msg_dest = ADDR_CAN_ODB;
    msg.msg_source = bmsAddress.CANAddress;
    msg.msg_comm_iface = NETV_COMM_IFACE_CAN1;
    msg.msg_remote = 0;
    msg.msg_data_length = 0;
    msg.msg_data[0] = m_state;

    netv_send_message(&msg);

    msg.msg_type = VUE32_TYPE_SETVALUE;
    msg.msg_dest = 0x03;
    netv_send_message(&msg);
}

//--------------------------------------------------------------------------------------------------------------
// Some states implementation
void monitor(void) {
    status(&branch0); // Mise à jour des status

    updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise à jour des tensions et de la température

    if(branch0.cellBalancing.value != 0)
    {
            branch0.cellBalancing.value = 0;
	    branch0.cellBalancing.changed = 1;
    }

    /*if (verifyTensionMin() || verifyTemperatureCell())
    {
        CANTransmetOpenContactor();
        setState(ProblemDetected);
    }*/

}

void balance(void) {
    status(&branch0); // Mise à jour des status

    if(isCellBleeding(&branch0) == 0)
    {
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise à jour des tensionn et de la température

        if (verifyTensionMax() || verifyTemperatureCell()) {
            CANTransmetOpenContactor();
            setState(WaitStabiliseTension);
            return;
        }
        if (verifyTemperatureRes()) {
            CANTransmetOpenContactor();
            setState(WaitStabiliseTemp);
            return;
        }

        updateFETs(&branch0); // Mise à jour des FETs pour le balancement

    }
}




//--------------------------------------------------------------------------------------------------------------
// General functions

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

    // Send error state message through CAN network
    EVERY_X_MS(1000)
    CANTransmetErrorState();
    END_OF_EVERY

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
    delayTime(20000000);

    writeRegister(branch0.id, BROADCAST_ADDRESS, IO_CONTROL, 0x00); //Close all LED
    writeRegister(branch0.id, BROADCAST_ADDRESS, CB_CTRL, 0x00); //Close all FET
    //toggleLed1();
    //toggleLed2();

    delayTime(100);

}
//TODO Put emergency instructions here
void OnEmergencyMsgBMS(void)
{
    return;
}

void InitBMS(void)
{
    return;
}

void initializePeripheral(void) {
    //initialiseIO(); //Initialiser les IO
    initAddress(); //Initialiser les addresses
    InitializeSPI(); //Initialiser la communication SPI

}

void initAddress(void) {
    // TODO: Uncomment
    //bmsAddress.address = getAddress();
    bmsAddress.address = 0x11;
    //bmsAddress.branchAddress = getBranchAddress();
    bmsAddress.branchAddress = 0x01;
    //bmsAddress.CANAddress = PORTE ^ 0x3F;
    bmsAddress.CANAddress = 0x11;
}

ROUTING_TABLE *gRoutingTableBMS_0 = NULL;