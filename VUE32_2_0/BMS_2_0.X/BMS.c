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

HDW_MAPPING gBMS_Ress[] =
{
    {E_ID_BMS_BOARD_TEMP, 2, 0x00},
    {E_ID_BMS_CELL_GROUP1, 7, 0x00},
    {E_ID_BMS_CELL_GROUP2, 7, 0x00},
};

#define NB_ROUND_BEFORE_SEND 2
#define NB_ROUND_BEFORE_SEND_STATUS 10

// Local variables
unsigned int statusCmp;
unsigned int bufferMoy;
enum eStates m_state, lastState;
int sendData;	//Compteur pour le delay pour envoyer sur le CAN

// From CANFunctions.c
extern BMSAddress bmsAddress;
extern enum eStates m_state;
unsigned char changeTensionReceive = 0; //Permet d'envoyer un Acknoledge quand les 2 parties du messages sont recu

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
    int cmd;
    cmd = msg->msg_cmd;
    // Deal with SETVALUE requests
    switch(msg->msg_type)
    {
        case VUE32_TYPE_SETVALUE:
        {
            if(cmd == E_ID_BMS_STATE_ERROR)
            {
                setState(Monitor);

                resetBranchFault(&branch0); // Reset des fautes
                resetBranchAlert(&branch0); // Reset des alertes
            }
            else if(cmd == E_ID_BMS_TENSIONS && msg->msg_data_length == (4*sizeof(float)))
            {
                tensionMaxCell = ((float*)msg->msg_data)[0];
                gapOverMinCell = ((float*)(msg->msg_data+sizeof(float)))[0];  
                tensionStartFet = ((float*)(msg->msg_data+(2*sizeof(float))))[0];
                tensionMinCell = ((float*)(msg->msg_data+(3*sizeof(float))))[0];

            }
            else if(cmd == E_ID_BMS_TEMPERATURES && msg->msg_data_length == (3*sizeof(float)))
            {
                temperatureMaxCell = ((float*)msg->msg_data)[0];
                temperatureMaxRes = ((float*)(msg->msg_data+sizeof(float)))[0];
                branch0.temperatureMaxResistance.changed = 1;
                tensionMaxOpenContactor = ((float*)(msg->msg_data+sizeof(float)+sizeof(float)))[0];
            }
            else if(cmd == E_ID_BMS_STATE_BALANCING && msg->msg_data_length == 1)
            {
                if(msg->msg_data[0] == 1)
                {
                    setState(Balance);
                }
                else
                {
                    setState(Monitor);
                }
            }
            break;
        }
            
        case VUE32_TYPE_GETVALUE:
        {
            char add;
            if(cmd == E_ID_BMS_BOARD_TEMP)
            {
                msg->msg_type = msg->msg_type + 1;
                msg->msg_remote = 0;
                add = msg->msg_dest;
                msg->msg_dest = msg->msg_source;
                if ( msg->msg_source == 0x3F )
                    msg->msg_source = GetMyAddr();
                else
                    msg->msg_source = add;
                    msg->msg_data_length = sizeof(float);

                    branch0.temperatureMaxResistance.changed = 0;
                    msg->msg_data[0] = 0xFF & branch0.temperatureMaxResistance.value;
                    msg->msg_data[1] = 0xFF & (branch0.temperatureMaxResistance.value >> 8);
                    netv_send_message(msg);
            }
            else if(cmd == E_ID_BMS_CELL_GROUP1)
            {
                msg->msg_type = msg->msg_type + 1;
                msg->msg_remote = 0;
                add = msg->msg_dest;
                msg->msg_dest = msg->msg_source;
                if ( msg->msg_source == 0x3F )
                    msg->msg_source = GetMyAddr();
                else
                    msg->msg_source = add;
                    
                    // Send Cells 1 and 2 voltages and device temperature
                    msg->msg_data_length = (3*sizeof(float)+8);

                    branch0.deviceTable[1].cellTable[0].tension.changed = 0;
                    branch0.deviceTable[1].cellTable[1].tension.changed = 0;


                    // CellGroup 1 resource ID
                    msg->msg_data[0] = (0xFF & CG_ID_BMS_CELLGROUP_VOLT1_TEMP);

                    // CellGroup 1 Cell 1
                    msg->msg_data[1] = 0xFF & branch0.deviceTable[1].cellTable[0].tension.value;
                    msg->msg_data[2] = 0xFF & (branch0.deviceTable[1].cellTable[0].tension.value >> 8);

                    // CellGroup 1 Cell 2
                    msg->msg_data[3] = 0xFF & branch0.deviceTable[1].cellTable[1].tension.value;
                    msg->msg_data[4] = 0xFF & (branch0.deviceTable[1].cellTable[1].tension.value >> 8);

                    // CellGroup 1 Temperature
                    msg->msg_data[5] = 0xFF & branch0.deviceTable[1].temperature1.value;
                    msg->msg_data[6] = 0xFF & (branch0.deviceTable[1].temperature1.value >> 8);

                    netv_send_message(msg);

                    // Send Cell 3 and 4 voltages
                    msg->msg_data_length = (2*sizeof(float)+8);

                    branch0.deviceTable[1].cellTable[2].tension.changed = 0;
                    branch0.deviceTable[1].cellTable[3].tension.changed = 0;
                    
                    // CellGroup 1 Resource ID
                    msg->msg_data[0] = (0xFF & CG_ID_BMS_CELLGROUP_VOLT2);
                    
                    // CellGroup 1 Cell 3
                    msg->msg_data[1] = 0xFF & branch0.deviceTable[1].cellTable[2].tension.value;
                    msg->msg_data[2] = 0xFF & (branch0.deviceTable[1].cellTable[2].tension.value >> 8);
                    
                    // CellGroup 1 Cell 4
                    msg->msg_data[3] = 0xFF & branch0.deviceTable[1].cellTable[3].tension.value;
                    msg->msg_data[4] = 0xFF & (branch0.deviceTable[1].cellTable[3].tension.value >> 8);

                    netv_send_message(msg);
            }
            else if(cmd == E_ID_BMS_CELL_GROUP2)
            {
                msg->msg_type = msg->msg_type + 1;
                msg->msg_remote = 0;
                add = msg->msg_dest;
                msg->msg_dest = msg->msg_source;
                if ( msg->msg_source == 0x3F )
                    msg->msg_source = GetMyAddr();
                else
                    msg->msg_source = add;

                    // Send Cells 5 and 6 voltages and device temperature
                    msg->msg_data_length = (3*sizeof(float)+8);

                    branch0.deviceTable[2].cellTable[0].tension.changed = 0;
                    branch0.deviceTable[2].cellTable[1].tension.changed = 0;

                    // CellGroup 2 Resource ID
                    msg->msg_data[0] = (0xFF & CG_ID_BMS_CELLGROUP_VOLT1_TEMP);

                    // CellGroup 2 Cell 1
                    msg->msg_data[1] = 0xFF & branch0.deviceTable[2].cellTable[0].tension.value;
                    msg->msg_data[2] = 0xFF & (branch0.deviceTable[2].cellTable[0].tension.value >> 8);

                    // CellGroup 2 Cell 2
                    msg->msg_data[3] = 0xFF & branch0.deviceTable[2].cellTable[1].tension.value;
                    msg->msg_data[4] = 0xFF & (branch0.deviceTable[2].cellTable[1].tension.value >> 8);

                    // CellGroup 2 Temperature
                    msg->msg_data[5] = 0xFF & branch0.deviceTable[2].temperature1.value;
                    msg->msg_data[6] = 0xFF & (branch0.deviceTable[2].temperature1.value >> 8);

                    netv_send_message(msg);

                    // Send Cell 7 and 8 voltages
                    msg->msg_data_length = (2*sizeof(float)+8);

                    branch0.deviceTable[2].cellTable[2].tension.changed = 0;
                    branch0.deviceTable[2].cellTable[3].tension.changed = 0;

                    // CellGroup 2 resource ID
                    msg->msg_data[0] = (0xFF & CG_ID_BMS_CELLGROUP_VOLT2);

                    // CellGroup 2 Cell 3
                    msg->msg_data[1] = 0xFF & branch0.deviceTable[2].cellTable[2].tension.value;
                    msg->msg_data[2] = 0xFF & (branch0.deviceTable[2].cellTable[2].tension.value >> 8);

                    // CellGroup 2 Cell 4
                    msg->msg_data[3] = 0xFF & branch0.deviceTable[2].cellTable[3].tension.value;
                    msg->msg_data[4] = 0xFF & (branch0.deviceTable[2].cellTable[3].tension.value >> 8);

                    netv_send_message(msg);
            }
            break;
        }

        default:
            
            break;
    }
}

//--------------------------------------------------------------------------------------------------------------
//CAN transmission functions
void CANTransmetErrorState()
{
    NETV_MESSAGE msg;

    msg.msg_priority = NETV_PRIORITY_MEDIUM;
    msg.msg_type = VUE32_TYPE_GETVALUE;
    msg.msg_cmd = E_ID_BMS_STATE_ERROR;
    msg.msg_dest = ADDR_CAN_ODB;
    msg.msg_source = bmsAddress.CANAddress & 0x3F;
    msg.msg_data_length = 1;

    msg.msg_data[0] = m_state;

    netv_send_message(&msg);
}

void CANTransmetOpenContactor(void) {

    NETV_MESSAGE msg;

    msg.msg_priority = NETV_PRIORITY_HIGHEST;
    msg.msg_type = NETV_TYPE_EMERGENCY;
    msg.msg_cmd = E_ID_BMS_OPEN_CONTACT;
    msg.msg_dest = ADDR_CAN_ODB;
    msg.msg_source = bmsAddress.CANAddress & 0x3F;
    msg.msg_data_length = 0;

    msg.msg_data[0] = m_state;

    netv_send_message(&msg);
}

void CAN1RxMsgProcess()
{
    unsigned int tmp = 0;
    NETV_MESSAGE g_rMessage; // pointeur vers une FIFO

    //-------------------------------------------------------
    //Si c'est un message pour le controle du BMS par la télémétrie
	if(netv_transceiver(bmsAddress.CANAddress,&g_rMessage))
	{

	    if (g_rMessage.msg_dest == E_ID_BMS_STATUS)
		{
	        //si le bit de commande de reset des BQ est activé
	        setState(g_rMessage.msg_data[0]);
	    }
		else if (g_rMessage.msg_dest == E_ID_BMS_TENSIONS)
		{
	        tmp = g_rMessage.msg_data[0] << 8;
	        tmp = tmp | (g_rMessage.msg_data[1]);
	        tensionMaxCell = tmp;
	        tmp = g_rMessage.msg_data[2] << 8;
	        tmp = tmp | (g_rMessage.msg_data[3]);
	        gapOverMinCell = tmp;
	        tmp = g_rMessage.msg_data[4] << 8;
	        tmp = tmp | (g_rMessage.msg_data[5]);
	        tensionStartFet = tmp;
	        tmp = g_rMessage.msg_data[6] << 8;
	        tmp = tmp | (g_rMessage.msg_data[7]);
	        tensionMinCell = tmp;

	        changeTensionReceive = 1;
	    }
            else if (g_rMessage.msg_dest == E_ID_BMS_TEMPERATURES)
            {
                tmp = g_rMessage.msg_data[0] << 8;
                tmp = tmp | (g_rMessage.msg_data[1]);
                temperatureMaxCell = tmp;
                tmp = g_rMessage.msg_data[2] << 8;
                tmp = tmp | (g_rMessage.msg_data[3]);
                temperatureMaxRes = tmp;

                tmp = g_rMessage.msg_data[4] << 8;
                tmp = tmp |(g_rMessage.msg_data[5]);
                tensionMaxOpenContactor = tmp;

                saveDataToMemory();

	    }
	}
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

    if (verifyTensionMin() || verifyTemperatureCell())
    {
        CANTransmetOpenContactor();
        setState(ProblemDetected);
    }

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
    CANTransmetErrorState();

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
    delayTime(100);

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

ROUTING_TABLE *gRoutingTableBMS = NULL;