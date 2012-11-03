/*************************************************************************
 *
 *   
 *
 *    File name   :  io.h
 *    Description :  Controler les IO du pic
 *
 *    History :
 *    1. Date        : 	10-03-2012
 *       Author      : 	Joel Viau
 *       Description : 
 *
 **************************************************************************/



void initialiseIO();
unsigned char getAddress();
unsigned char getBranchAddress();
void toggleLed1();
void toggleLed2();
void toggleIo1();
void toggleIo2();