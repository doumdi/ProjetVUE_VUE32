
#include "HardwareProfile.h"
#include "Board.h"
#include "BMS.h"
#include "../src/NETV32_Common.h"
#include "../src/NETV32_CANDriver.h"
#include "../src/VUE32_Utils.h"
#include "../src/communication.h"

//Comments:
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//=> Do a Search in Files (CTRL+SHIFT+F) with "ToDo" to find incomplete
//   statements.
//=> At the end of this file there is a listing of board specific functions.


//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                    Constants and variables                               //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

//interrupts.c
extern volatile unsigned int flag_1ms_a, flag_1ms_b;
unsigned int flag_fsm = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                         Prototypes                                       //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

//OpenECoSys NetV
void init_default_variables(void);
void netv_proc_message(NETV_MESSAGE *message);
void update_variables(void);

void InitBMS(void);
void CallBMSImpl(void);
void ImplBMS();

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                        Main function                                     //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    InitBoard();

    //Set ErrorStateFlag
    ErrorStateFlag = FALSE;
    
    m_state = InitPeripheral;
    //LED2 ^= 1;

    //Most of the functions in the while(1) loop are timed by Timer1
    while (1)
    {
        // Process state machine        
        ImplBMS();

        ClearWDT();
        //CallBMSImpl();        
        
        
        /*NETV_MESSAGE oMsgRecep;

        EVERY_X_MS(1000)
            
            oMsgRecep.msg_cmd = 0;
            oMsgRecep.msg_comm_iface =0xFF;
            oMsgRecep.msg_data_length = 0;
            oMsgRecep.msg_dest = 0xFF;
            oMsgRecep.msg_remote = 1;
            oMsgRecep.msg_source = GetMyAddr();
            oMsgRecep.msg_type = 0x80;
            oMsgRecep.msg_priority = 1;
            netv_send_message(&oMsgRecep );
            LED1 ^= 1;
        END_OF_EVERY


        if(netv_transceiver((unsigned char)GetBoardID(), &oMsgRecep))
        {
            //LED2 ^= 1;
            OnMsgBMS(&oMsgRecep);
        }*/
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                           Functions                                      //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////


//OpenECoSys Network Viewer:
//==========================
/*
void init_default_variables(void)
{
    memset(&g_globalNETVVariables, 0, sizeof(GlobalNETVVariables));
}

void update_variables(void)
{
    //Board ID
    g_globalNETVVariables.vue32_id = GetBoardID() ;

    //Onboard sensors
    g_globalNETVVariables.board_temp = board_temp;
    g_globalNETVVariables.board_volt = board_volt;

<<<<<<< .mine
}*/

//Config fuses
// SYSCLK = (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// SYSCLK = (8MHz Crystal/ 2 * 20 / 1) = 80MHz
// PBCLK = SYSCLK = 80MHz

//#pragma config FFRCDIV   = 0             // FRC Postscaler = 1
//#pragma config FCOSC     = FRCPLL        // Use interface osc with PLL
#pragma config UPLLEN   = ON        	// USB PLL Enabled
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1        // Peripheral Clock divisor
//#pragma config FROSEL    = FRC
//#pragma config FRODIV    = 0
#pragma config FWDTEN   = ON           // Watchdog Timer
#pragma config WDTPS    = PS2048       // Watchdog Timer Postscale = 2s
#pragma config FCKSM    = CSECMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = FRCPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = ON            // Background Debugger Enable
#pragma config FVBUSONIO = OFF			// VBUS_ON pin is controlled by the Port Function
#pragma config FUSBIDIO = OFF			// USBID pin is controlled by the Port Function
#pragma config FSRSSEL = PRIORITY_7		//SRS Interrupt Priority Level 7

#pragma config FCANIO = ON