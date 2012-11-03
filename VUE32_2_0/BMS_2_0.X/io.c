/*************************************************************************
 *
 *   
 *
 *    File name   :  io.c
 *    Description :  Controler les IO du pic
 *
 *    History :
 *    1. Date        : 	10-03-2012
 *       Author      : 	Joel Viau
 *       Description : 
 *
 **************************************************************************/

#include "plib.h"

void initialiseIO()
{
	//Led Fault
	mPORTBSetPinsDigitalOut(BIT_8);
	PORTSetBits(IOPORT_B, BIT_8);
	//Led Alert
	mPORTBSetPinsDigitalOut(BIT_9);
	PORTSetBits(IOPORT_B, BIT_9);

	//Relais 1
	mPORTDSetPinsDigitalOut(BIT_1);
	PORTSetBits(IOPORT_D, BIT_1);
	
	//Relais 2
	mPORTDSetPinsDigitalOut(BIT_2);
	PORTSetBits(IOPORT_D, BIT_2);
	
	//Address
	mPORTESetPinsDigitalIn(BIT_0);
	mPORTESetPinsDigitalIn(BIT_1);
	mPORTESetPinsDigitalIn(BIT_2);
	mPORTESetPinsDigitalIn(BIT_3);
	mPORTESetPinsDigitalIn(BIT_4);
	mPORTESetPinsDigitalIn(BIT_5);
	mPORTESetPinsDigitalIn(BIT_6);
	mPORTESetPinsDigitalIn(BIT_7);	
}

unsigned char getAddress()
{
	unsigned char address = (PORTE & 0x0F) ^ 0x0F;
	return address;
}
unsigned char getBranchAddress()
{
	unsigned char branche = ((PORTE >> 4) & 0x01) ^ 0x01;
	return branche;
}

void toggleLed1()
{
	mPORTBToggleBits(BIT_8);
}
void toggleLed2()
{
	mPORTBToggleBits(BIT_9);
}
void toggleIo1()
{
	mPORTDToggleBits(BIT_1);
}
void toggleIo2()
{
	mPORTDToggleBits(BIT_2);
}
