
#include "HardwareProfile.h"
#include "Board.h"
#include "BMS.h"
#include "../src/NETV32_Common.h"
#include "../src/VUE32_Utils.h"

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

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                        Main function                                     //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{

    // Initialize the board (communication, timers, etc).
    InitBoard();

    // Get the actual board ID
    //VUE32_ID id = GetBoardID();

    // Specific VUE32 initialization
    //InitBMS();

    //Set ErrorStateFlag
    ErrorStateFlag = FALSE;
    
    m_state = InitPeripheral;

    //Most of the functions in the while(1) loop are timed by Timer1
    int i = 1;

    while (i <= 2)
    {
        // Process state machine
        CallBMSImpl();
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

#pragma config UPLLEN   = OFF        	// USB PLL Enabled
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config UPLLIDIV = DIV_1         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_32         // PLL Output Divider
#pragma config FPBDIV   = DIV_1        // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1024           // Watchdog Timer Postscale
//#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
//#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = ON           // Internal/External Switch-over
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