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

// From CANFunctions.c
extern BMSAddress bmsAddress;
extern enum eStates m_state;
unsigned char changeTensionReceive = 0; //Permet d'envoyer un Acknoledge quand les 2 parties du messages sont recu

// Prototypes
void balance(void);
void monitor(void);
void openContactor(void);
void openCloseAllIO(void);

void delayTime(unsigned int time) //0 � 536 000ms
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
    // Gestion du fonctionnement de la machine a �tat
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
            initBranchData(&branch0, 0); // Met les valeurs � z�ro
            assignAddress(&branch0); // Assigne les address au device
            status(&branch0); // Mise � jour des registres de status des devices
            setInitialConfig(&branch0); // Assigne la configuration initiale
            resetBranchFault(&branch0); // Reset des fautes
            resetBranchAlert(&branch0); // Reset des alertes
            status(&branch0); // Mise � jour des status

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
            wakeUpBranch(&branch0); //R�initialise les BQ apr�s le mode sleep
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
                    //ToDo: Set msg_data to tempsRes
                    //((float*)msg->msg_data)[0] = ;
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
                msg->msg_data_length = (3*sizeof(float));
                //ToDo: Set data values (cell voltage 1, cell voltage 2 and temperature)
                //((float*)msg->msg_data)[0] = ;
                //((float*)(msg->msg_data+sizeof(float)))[0] = ;
                //((float*)(msg->msg_data+(2*sizeof(float))))[0] = ;
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
                msg->msg_data_length = (2*sizeof(float));
                //ToDo: Set data values (cell voltage 3, cell voltage 4)
                //((float*)msg->msg_data)[0] = ;
                //((float*)(msg->msg_data+sizeof(float)))[0] = ;
                netv_send_message(msg);
            }
            break;
        }

        default:
            
            break;
    }
}



//--------------------------------------------------------------------------------------------------------------

void monitor(void) {
    status(&branch0); // Mise � jour des status
    updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise � jour des tensions et de la temp�rature

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
        CANTransmetBranch(&branch0, bmsAddress.CANAddress); //Envoie les donn�es de tensions et temp�rature sur le CAN
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les donn�es de satus sur le CAN

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
    //Si la temp�rature est au dessus de la temp�rature maximale des cellules
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
    status(&branch0); // Mise � jour du status

    if(isCellBleeding(&branch0) == 0)
    {
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise � jour des tensions et de la temp�rature

        if (verifyTemperatureRes()) {
            setState(WaitStabiliseTemp);
            return 0;
        }
        updateFETs(&branch0); // Mise � jour des fets
    }
    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress);
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les donn�es de satus sur le CAN

    //Si la tension max est de retour � la tension ajust�, on change d'�tat pour retourner � l'�tat pr�c�dent
    if (branch0.tensionMax <= (tensionMaxOpenContactor - 50) ) {
        return 1;
    }
    return 0;
}

int problemDetected()
{
    status(&branch0); // Mise � jour du status
    updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise � jour des tensions et de la temp�rature

    if(branch0.cellBalancing.value != 0)
    {
            branch0.cellBalancing.value = 0;
	    branch0.cellBalancing.changed = 1;
    }

    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress);
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les donn�es de satus sur le CAN

    //Si la tension max est de retour � la tension ajust�, on change d'�tat pour retourner � balance
    if (!verifyTensionMin() && !verifyTemperatureCell()) {
        return 1;
    }
    return 0;
}

int waitStabiliseTemp() {
    status(&branch0); // Mise � jour du status
    updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise � jour des tensions et de la temp�rature

    if(branch0.cellBalancing.value != 0)
    {
            branch0.cellBalancing.value = 0;
	    branch0.cellBalancing.changed = 1;
    }

    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress);
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les donn�es de satus sur le CAN

    //Si la tension max est de retour � la tension ajust�, on change d'�tat pour retourner � balance
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

////////////////////////////////////////////////////////////////////////////////
//From CANFunction.c
////////////////////////////////////////////////////////////////////////////////

void CANTransmetTension(Branch* branch, unsigned short address)
{
	int cellNb = 0;
	int deviceNb = 0;
	int dataNb = 0;

	NETV_MESSAGE msg;

//   unsigned char msg_priority;      // 1 bit
//   unsigned char msg_type;          // 8 bits
//   unsigned char msg_cmd;           // 8 bits
//   unsigned char msg_dest;          // 6 bits
//   unsigned char msg_source;        // 6 bits
//   unsigned char msg_data_length;
//   unsigned char msg_data[8];

	msg.msg_priority = NETV_PRIORITY_MEDIUM;
	msg.msg_type = VUE32_TYPE_GETVALUE;
        msg.msg_cmd = 0; // Todo:CG_ID_BMS_CELLGROUP_VOLT1_TEMP;
        msg.msg_dest = ADDR_CAN_ODB;
	msg.msg_source = address & 0x3F;
	msg.msg_data_length = 7;

	for (deviceNb = 0; deviceNb < nbDevice; deviceNb++)
	{
		msg.msg_data[dataNb++] = (0xFF & E_ID_BMS_CELL_GROUP1) + deviceNb;

		for (cellNb = 0; cellNb < 2; cellNb++)
		{
		    branch->deviceTable[deviceNb].cellTable[cellNb].tension.changed = 0;
		    msg.msg_data[dataNb++] = 0xFF & branch->deviceTable[deviceNb].cellTable[cellNb].tension.value;
		    msg.msg_data[dataNb++] = 0xFF & (branch->deviceTable[deviceNb].cellTable[cellNb].tension.value >> 8);
		}

		msg.msg_data[dataNb++] = 0xFF & branch->deviceTable[deviceNb].temperature1.value;
		msg.msg_data[dataNb++] = 0xFF & (branch->deviceTable[deviceNb].temperature1.value >> 8);

		//can_netv_send_message(&msg, CAN1);


		msg.msg_priority = NETV_PRIORITY_MEDIUM;
                msg.msg_type = VUE32_TYPE_GETVALUE;
                msg.msg_cmd = 0; // Todo: CG_ID_BMS_CELLGROUP_VOLT2;
                msg.msg_dest = ADDR_CAN_ODB;
                msg.msg_source = address & 0x3F;
                msg.msg_data_length = 5;

		dataNb = 0;


		msg.msg_data[dataNb++] = (0xFF & E_ID_BMS_CELL_GROUP1) + deviceNb;

		for (cellNb = 2; cellNb < 4; cellNb++)
		{
		    branch->deviceTable[deviceNb].cellTable[cellNb].tension.changed = 0;
		    msg.msg_data[dataNb++] = 0xFF & branch->deviceTable[deviceNb].cellTable[cellNb].tension.value;
		    msg.msg_data[dataNb++] = 0xFF & (branch->deviceTable[deviceNb].cellTable[cellNb].tension.value >> 8);
		}

		//can_netv_send_message(&msg, CAN1);
	}


}

void CANTransmetTempBal(Branch* branch, unsigned short address)
{
	int dataNb = 0;

	NETV_MESSAGE msg;


	msg.msg_priority = NETV_PRIORITY_MEDIUM;
	msg.msg_type = VUE32_TYPE_GETVALUE;
        msg.msg_cmd = E_ID_BMS_BOARD_TEMP;
        msg.msg_dest = ADDR_CAN_ODB;
	msg.msg_source = address & 0x3F;
	msg.msg_data_length = 4;


	branch->temperatureMaxResistance.changed = 0;
	branch->cellBalancing.changed = 0;

	msg.msg_data[dataNb++] = 0xFF & branch->temperatureMaxResistance.value;
	msg.msg_data[dataNb++] = 0xFF & (branch->temperatureMaxResistance.value >> 8);
	msg.msg_data[dataNb++] = 0xFF & branch->cellBalancing.value;				//
	msg.msg_data[dataNb++] = 0xFF & (branch->cellBalancing.value >> 8);


	//can_netv_send_message(&msg, CAN1);

//   D1             D2
//V1 V2 TD       V5 V6 TD
//V3 V4          V7 V8
//TR  BAL

}

//void CANTransmetTensionMinMax(Branch* branch, unsigned short address, unsigned char send)
//{
//    //D�finition d'un pointeur vers une pile
///*
//	NETV_MESSAGE msg;
//
//
//	msg.msg_priority = 0x00;
//	msg.msg_type = NETV_TYPE_SENSOR_LOW_PRIORITY;
//	msg.msg_cmd = NETV_EVENTS_CMD_ALIVE;
//	msg.msg_dest = address;
//	msg.msg_data_length = 6;
//
//	msg.msg_data[0] = 0xFF & branch->tensionMin;
//    msg.msg_data[1] = 0xFF & (branch->tensionMin >> 8);
//    msg.msg_data[2] = 0xFF & branch->tensionMax;
//    msg.msg_data[3] = 0xFF & (branch->tensionMax >> 8);
//    msg.msg_data[4] = branch->tensionMinNb;
//    msg.msg_data[5] = branch->tensionMaxNb;
//
//	netv_send_message(&msg);
//*/
//
//}


//----------------------------------------------------------------------------------------------

void CANTransmetStatus()
{
    NETV_MESSAGE msg;

    msg.msg_priority = NETV_PRIORITY_MEDIUM;
    msg.msg_type = VUE32_TYPE_GETVALUE;
    msg.msg_cmd = E_ID_BMS_STATUS;
    msg.msg_dest = ADDR_CAN_ODB;
    msg.msg_source = bmsAddress.CANAddress & 0x3F;
    msg.msg_data_length = 1;

    msg.msg_data[0] = m_state;

    //can_netv_send_message(&msg, CAN1);

}

//----------------------------------------------------------------------------------------------

void CANTransmetOpenContactor()
{

    NETV_MESSAGE msg;


    msg.msg_priority = NETV_PRIORITY_HIGHEST;
    msg.msg_type = NETV_TYPE_EMERGENCY;
    msg.msg_cmd = E_ID_BMS_OPEN_CONTACT;
    msg.msg_dest = ADDR_CAN_ODB;
    msg.msg_source = bmsAddress.CANAddress & 0x3F;
    msg.msg_data_length = 0;

    msg.msg_data[0] = m_state;

    //can_netv_send_message(&msg, CAN1);

}


void CANTransmetBranch(Branch *branch, unsigned short address)
{

    CANTransmetTension(branch, address);
    CANTransmetTempBal(branch, address);
//    CANTransmetTensionMinMax(branch, address, send);
}

//--------------------------------------------------------------------------------------------------------------

void balance(void) {
    status(&branch0); // Mise � jour des status

    if(isCellBleeding(&branch0) == 0)
    {
        updateCellVoltage(&branch0,bufferMoy%BUFFER_MOY); // Mise � jour des tensionn et de la temp�rature

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


        updateFETs(&branch0); // Mise � jour des FETs pour le balancement

    }
    if (sendData++ % NB_ROUND_BEFORE_SEND == 0) {
        CANTransmetBranch(&branch0, bmsAddress.CANAddress); //Envoie les donn�es de tensions et temp�rature sur le CAN
    }
    if (statusCmp++ % NB_ROUND_BEFORE_SEND_STATUS)
        CANTransmetStatus(&branch0); //Envoie les donn�es de satus sur le CAN
}

//========================================================================================================================
//
//                                             R�CEPTION du CAN1
//
//========================================================================================================================

//------------------------------------------------------------------------------
//                       Interruption du port CAN 1
//------------------------------------------------------------------------------
/*
void __attribute__((vector(46), interrupt(ipl4), nomips16)) CAN1InterruptHandler(void) {
    // si l'interruption est pour la r�ception du CAN1
    if ((CANGetModuleEvent(CAN1) & CAN_RX_EVENT) != 0) {
        // si c'est une r�ception d'un message standard
        if (CANGetPendingEventCode(CAN1) == CAN1_RX_FIFO_PRIORITAIRE_EVENT) {
            // D�sactive l'interruption de la FIFO standard pour le traitement message
            // Cette interruption est r�activ� apres le traiment du message dans
            CANEnableChannelEvent(CAN1, CAN1_RX_FIFO_PRIORITAIRE, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN1MsgPrioritaireRecu = TRUE; //Flag pour que le main traite le message
        }
    }
    INTClearFlag(INT_CAN1);
}*/

//------------------------------------------------------------------------------

void CAN1RxMsgProcess()
{
    unsigned int tmp = 0;
    NETV_MESSAGE g_rMessage; // pointeur vers une FIFO

    //-------------------------------------------------------
    //Si c'est un message pour le controle du BMS par la t�l�m�trie
	if(netv_transceiver(bmsAddress.CANAddress,&g_rMessage))
	{

	    if (g_rMessage.msg_dest == E_ID_BMS_STATUS)
		{
	        //si le bit de commande de reset des BQ est activ�
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

//------------------------------------------------------------------------------

void CAN1RxNormalMsgProcess() {

}

//------------------------------------------------------------------------------

void CANSendAck()
{

    NETV_MESSAGE msg;


    msg.msg_priority = NETV_PRIORITY_MEDIUM;
    msg.msg_type = VUE32_TYPE_GETVALUE;
    msg.msg_cmd = E_ID_BMS_STATUS;
    msg.msg_dest = ADDR_CAN_ODB;
    msg.msg_source = bmsAddress.CANAddress & 0x3F;
    msg.msg_data_length = 1;

    //can_netv_send_message(&msg, CAN1);
}

