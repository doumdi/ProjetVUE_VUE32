
#include "HardwareProfile.h"
#include "Board.h"
#include "NETV32_Common.h"
#include "VUE32_Utils.h"

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

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                        Main function                                     //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    // Get the actual board ID
    VUE32_ID id = GetBoardID();

    // Initialize the board (communication, timers, etc).
    InitBoard();

    // Specific VUE32 initialization
    InitVUE32(id);

    //Most of the functions in the while(1) loop are timed by Timer1
    while (1)
    {
        EVERY_X_MS(125)
           LED1 ^= 1;  //Toggle LED 4Hz
        END_OF_EVERY

        // Process network stack
        NETV_MESSAGE oMsgRecep;
	if(netv_transceiver((unsigned char)id, &oMsgRecep))
            OnMsgVUE32(&oMsgRecep);

        // Process state machine
        CallVUE32Impl(id);
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
#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = ON           	// Debugger enabled

/*
 * Board specific functions.
 * See VUE32_Cables_and_Connectors for a complete description.
 *
 *VUE32 #1:
 *=========
 * Serial Bridge - Do not use this code project
 *
 *VUE32 #2:
 *=========
 * Battery current sensing
 * GFI
 * 1x Speed sensor
 * 3x Power out (lights)
 *
 *VUE32 #3:
 *=========
 * CAN 2 (bridge) Drives
 * 2x Speed sensor
 * 2x Power Out (contactor and pump)
 *
 *VUE32 #4:
 *=========
 * Light lever
 * 4x Power out, including Wipers
 *
 *VUE32 #5:
 *=========
 * Brake and accelerator
 * D/P/R switch
 * Ignition key
 * CAN2 Steering wheel angle sensor
 * 1x Power output (amplifier)
 *
 *VUE32 #6:
 *=========
 * Lateral acceleration
 * Yaw rate
 * Wiper lever
 * 2x Power out (lights and washer pump)
 *
 *VUE32 #7:
 *=========
 * CAN2 BMS (bridge)
 * 1x Speed sensor
 * 3x Power out (lights)
 */
