/******************************************************************************
 * Hardware Profile Definitions
 *
 * All rights reserved to Projet VUE
 *
 * by Maxime Bedard - 24/09/2012
 * ****************************************************************************/

#ifndef _HARDWAREPROFILE_H_
#define _HARDWAREPROFILE_H_

#include <p32xxxx.h>

/*********************************************************************
* GetSystemClock() returns system clock frequency.
*
* GetPeripheralClock() returns peripheral clock frequency.
*
* GetInstructionClock() returns instruction clock frequency.
*
********************************************************************/

/*********************************************************************
* Macro: #define	GetSystemClock()
*
* Overview: This macro returns the system clock frequency in Hertz.
*			* value is 8 MHz x 4 PLL for PIC24
*			* value is 8 MHz/2 x 18 PLL for PIC32
*
********************************************************************/
    #if defined(__PIC24F__)
        #define GetSystemClock()    (32000000ul)
    #elif defined(__PIC32MX__)
        #define GetSystemClock()    (80000000ul)
    #elif defined(__dsPIC33F__) || defined(__PIC24H__)
        #define GetSystemClock()    (80000000ul)
    #elif defined(__dsPIC33E__) || defined(__PIC24E__)
    	#define GetSystemClock()    (120000000ul)
    #endif

/*********************************************************************
* Macro: #define	GetPeripheralClock()
*
* Overview: This macro returns the peripheral clock frequency
*			used in Hertz.
*			* value for PIC24 is <PRE>(GetSystemClock()/2) </PRE>
*			* value for PIC32 is <PRE>(GetSystemClock()/(1<<OSCCONbits.PBDIV)) </PRE>
*
********************************************************************/
    #if defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__) || defined(__dsPIC33E__) || defined(__PIC24E__)
        #define GetPeripheralClock()    (GetSystemClock() / 2)
    #elif defined(__PIC32MX__)
        #define GetPeripheralClock()    (GetSystemClock() / (1 << OSCCONbits.PBDIV))
    #endif

/*********************************************************************
* Macro: #define	GetInstructionClock()
*
* Overview: This macro returns instruction clock frequency
*			used in Hertz.
*			* value for PIC24 is <PRE>(GetSystemClock()/2) </PRE>
*			* value for PIC32 is <PRE>(GetSystemClock()) </PRE>
*
********************************************************************/
    #if defined(__PIC24F__) || defined(__PIC24H__) || defined(__dsPIC33F__) || defined(__dsPIC33E__) || defined(__PIC24E__)
        #define GetInstructionClock()   (GetSystemClock() / 2)
    #elif defined(__PIC32MX__)
        #define GetInstructionClock()   (GetSystemClock())
    #endif


/*********************************************************************/
/******************* Pin and Register Definitions ********************/
/*********************************************************************/

// Diagnostic LEDs (1 = Off (PNP driver))
#define LED1			LATBbits.LATB8
#define LED1_TRIS		TRISBbits.TRISB8
#define LED2			LATBbits.LATB9
#define LED2_TRIS		TRISBbits.TRISB9

// Relays (digital outputs)
#define RELAY1                  LATDbits.LATD2
#define RELAY1_TRIS             TRISDbits.TRISD2
#define RELAY2                  LATDbits.LATD1
#define RELAY2_TRIS             TRISDbits.TRISD1

// Digital Inputs (for the BMS Address)
#define DIO_TRIS		TRISE
#define DIO_LAT			LATE
#define DIO_PORT		PORTE
#define DIO_MASK                0x000000FF

//Digital I/Os - Board Specific pins
#define DIO0                    PORTEbits.RE0
#define DIO1                    PORTEbits.RE1
#define DIO2                    PORTEbits.RE2
#define DIO3                    PORTEbits.RE3
#define DIO4                    PORTEbits.RE4
#define DIO5                    PORTEbits.RE5
#define DIO6                    PORTEbits.RE6
#define DIO7                    PORTEbits.RE7
#define DIO0_TRIS               TRISEbits.TRISE0
#define DIO1_TRIS               TRISEbits.TRISE1
#define DIO2_TRIS               TRISEbits.TRISE2
#define DIO3_TRIS               TRISEbits.TRISE3
#define DIO4_TRIS               TRISEbits.TRISE4
#define DIO5_TRIS               TRISEbits.TRISE5
#define DIO6_TRIS               TRISEbits.TRISE6
#define DIO7_TRIS               TRISEbits.TRISE7

//CAN
#define CRX1_TRIS		TRISFbits.TRISF0	
#define CTX1_TRIS		TRISFbits.TRISF1

//SPI
#define SPI_CHN 	   	SPI_CHANNEL2
#define SPICLK_TRIS		TRISGbits.TRISG6
#define SPISDO_TRIS		TRISGbits.TRISG8
#define SPISDI_TRIS		TRISGbits.TRISG7
#define SPI_CS_TRIS 		TRISGbits.TRISG9
#define SPI_CS   		PORTGbits.RG9


#endif
