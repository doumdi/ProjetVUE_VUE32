/**********************************************************************
 * © 2009 Microchip Technology Inc.
 *
 * FileName:        CANFunctions.c
 * Dependencies:    Header (.h) files if applicable, see below
 * Processor:       PIC32
 * Compiler:        MPLAB® C32
 *
 * SOFTWARE LICENSE AGREEMENT:
 * Microchip Technology Incorporated ("Microchip") retains all ownership and
 * intellectual property rights in the code accompanying this message and in all
 * derivatives hereto.  You may use this code, and any derivatives created by
 * any person or entity by or on your behalf, exclusively with Microchip's
 * proprietary products.  Your acceptance and/or use of this code constitutes
 * agreement to the terms and conditions of this notice.
 *
 * CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
 * WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
 * PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
 *
 * YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE, WHETHER
 * IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF STATUTORY DUTY),
 * STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY INDIRECT, SPECIAL,
 * PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, FOR COST OR EXPENSE OF
 * ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN
 * ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO
 * THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
 * HAVE THIS CODE DEVELOPED.
 *
 * You agree that you are solely responsible for testing the code and
 * determining its suitability.  Microchip has no obligation to modify, test,
 * certify, or support the code.
 ************************************************************************/

//#include "CANFunctions.h"
//#include "error.h"
//#include "BMS.h"
//#include "BMS_Memory.h"
//#include "NETV32_Common.h"
//#include "NETV32_CANDriver.h"

// CAN du module de Tete
//static volatile BOOL CAN1MsgPrioritaireRecu = FALSE;
//static volatile BOOL CAN1MsgNormalRecu = FALSE;
//static volatile BOOL CAN1MsgOptionnelRecu = FALSE;

/// CAN du module Chef
//static volatile BOOL CAN2MsgRecu = FALSE;



//static volatile BOOL isCAN1MsgReceived = FALSE;

/* isCAN2MsgReceived is true if CAN2 channel 1 received
 * a message. This flag is updated in the CAN2 ISR. */
//static volatile BOOL isCAN2MsgReceived = FALSE;

//extern BMSAddress bmsAddress;
//extern enum eStates m_state;

//unsigned char changeTensionReceive = 0; //Permet d'envoyer un Acknoledge quand les 2 parties du messages sont recu

//------------------------------------------------------------------------------

/*void CANTransmetBranch(Branch *branch, unsigned short address)
{
 
    CANTransmetTension(branch, address);
    CANTransmetTempBal(branch, address);
//    CANTransmetTensionMinMax(branch, address, send);
}

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
//    //Définition d'un pointeur vers une pile
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

/*void CANTransmetStatus()
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



//========================================================================================================================
//
//                                             RÉCEPTION du CAN1
//
//========================================================================================================================

//------------------------------------------------------------------------------
//                       Interruption du port CAN 1
//------------------------------------------------------------------------------
/*
void __attribute__((vector(46), interrupt(ipl4), nomips16)) CAN1InterruptHandler(void) {
    // si l'interruption est pour la réception du CAN1
    if ((CANGetModuleEvent(CAN1) & CAN_RX_EVENT) != 0) {
        // si c'est une réception d'un message standard
        if (CANGetPendingEventCode(CAN1) == CAN1_RX_FIFO_PRIORITAIRE_EVENT) {
            // Désactive l'interruption de la FIFO standard pour le traitement message
            // Cette interruption est réactivé apres le traiment du message dans
            CANEnableChannelEvent(CAN1, CAN1_RX_FIFO_PRIORITAIRE, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);
            CAN1MsgPrioritaireRecu = TRUE; //Flag pour que le main traite le message
        }
    }
    INTClearFlag(INT_CAN1);
}*/

//------------------------------------------------------------------------------

/*void CAN1RxMsgProcess()
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
}*/













