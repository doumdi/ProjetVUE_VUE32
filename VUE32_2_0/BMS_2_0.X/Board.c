/******************************************************************************
 * Board support drivers
 * for initialisation and board specific functions
 * All rights reserved to Projet VUE
 *
 * by Maxime Bedard - 24/09/2012
 * ****************************************************************************/

#include "Board.h"
#include "HardwareProfile.h"
#include "NETV32_Common.h"
#include "VUE32_Utils.h"
#include "BMS_Memory.h"
#include "BMS_Impl.h"
#include <plib.h>

#define FIRMWARE_VERSION 0x0001

unsigned int m_unBoardId;

/*
 * Initialize the board
 * Timers, Communication, etc
 * Note : Should only be called once at the begginning of the main
 */
void InitBoard(void)
{
    // Initialize clock
    SYSTEMConfigPerformance(GetSystemClock());
    SYSTEMConfig(GetSystemClock(), SYS_CFG_PCACHE);
    SYSTEMConfig(GetSystemClock(), SYS_CFG_PB_BUS);
    SYSTEMConfigPB(GetSystemClock());
    INTEnableSystemMultiVectoredInt();

    //Disable JTAG port
    DDPCONbits.JTAGEN = 0;

    // Initialize LEDs
    LED1_TRIS = 0;
    LED2_TRIS = 0;

    // Initialize Timers
    initTimerBMS();

    // Initialize CAN bus
    CRX1_TRIS = 1;
    CTX1_TRIS = 0;
    netv_init_can_driver(GetBoardID(),CAN1);
    
    // Initialize digital IOs as inputs
    DIO_TRIS |= DIO_MASK;

    // Initialize Relays (low)
    RELAY1_TRIS = 0;
    RELAY2_TRIS = 0;
    RELAY1 = 0;
    RELAY2 = 0;
    
    // Initialize SPI pins as inputs
    SPICLK_TRIS = 1;
    SPISDO_TRIS	= 1;
    SPI_CS_TRIS = 1;
    SPISDI_TRIS	= 1;

    //TODO: Init unused pins as inputs
    
    // Read the board ID
    m_unBoardId = (DIO_PORT & DIO_MASK) ^ DIO_MASK;

    // Read the parameters previously saved in flash
    loadDataFromMemory();
    
    //Enables the core to handle any pending interrupt requests
    asm volatile ("ei"); 
}

/*
 * Get the persistent Board Number
 * This value is fixed in flash memory
 */
VUE32_ID GetBoardID(void)
{
    return (m_unBoardId & 0x000000FF);
}

/*
 * Get the firmware version
 */
unsigned short GetFirmVersion(void)
{
    return FIRMWARE_VERSION;
}

// Get our network address (depending of which VUE32 card we are)
unsigned char GetMyAddr()
{
    return (m_unBoardId & 0x000000FF);
}

/*
 * Initialize the specific VUE32 board
 * Drivers and unique functionnalities
 * Note : Should only be called only once at the begginning of the main
 */
/*void InitBMS(void)
{
    // This board is not a "true" VUE32 board, there is no specific initialization
    //gInitFunc[id]();
}*/

/*
 * Call the implementation of the main loop of a specific VUE32 board
 */
void CallBMSImpl(void)
{
    RunLongPolling();
    gImplFuncBMS[1]();
}

