/*************************************************************************
 *
 *   
 *
 *    File name   :  spi.h
 *    Description :  Méthodes reliées au SPI
 *
 *    History :
 *    1. Date        : 	11-12-2011
 *       Author      : 	Joel Viau
 *       Description : 
 *
 **************************************************************************/
#ifndef SPI_H
#define SPI_H

// Initialize SPI functions
void InitializeSPI(void);

// Toggle the SPI chip select
void toggleSPI_CS();

// Send a byte on the SPI bus
void sendByteSPI(unsigned char data);

// Receive a byte from SPI
// Todo: Make this non-blocking, involves a lot of changes
unsigned char getByteSPI();


#endif //SPI_H
