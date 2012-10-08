/*************************************************************************
 *
 *   
 *
 *    File name   :  spi.c
 *    Description :  Méthodes reliées au SPI
 *
 *    History :
 *    1. Date        : 	11-12-2011
 *       Author      : 	Joel Viau
 *       Description : 
 *
 **************************************************************************/

#include "Spi.h"
#include "HardwareProfile.h"
#include <plib.h>

// Todo: WTF?
void SpiInitDevice(SpiChannel chn, int isMaster, int frmEn, int frmMaster);

void InitializeSPI()
{
	
	SPI_CS_TRIS =0;	//Set pin as output

	SPI_CS = 1;// ensure SPI memory device


	SpiInitDevice(SPI_CHN, 1, 0, 0);

	ODCDbits.ODCD9 = 1;
	CNPUE |= 0xA00;
}

void SpiInitDevice(SpiChannel chn, int isMaster, int frmEn, int frmMaster)
{
	SpiOpenFlags	oFlags=SPI_OPEN_MODE8|SPI_OPEN_ENHBUF;	// SPI open mode
	if(isMaster)
	{
		oFlags|=SPI_OPEN_MSTEN;
	}
	if(frmEn)
	{
		oFlags|=SPI_OPEN_FRMEN;
		if(!frmMaster)
		{
			oFlags|=SPI_OPEN_FSP_IN;
		}
	}
	SpiChnOpen(chn, oFlags, 500);	// divide fpb by 4, configure the I/O ports. Not using SS in this example 700
//	SpiChnOpen(chn, oFlags, 500);	// divide fpb by 4, configure the I/O ports. Not using SS in this example 700

}

void toggleSPI_CS()
{
	int i;

	while(SPI2STAT & (1<<11));
	for(i=0;i<10;i++);	//delais

	SPI_CS = !SPI_CS;
}

void sendByteSPI(unsigned char data)
{
	SpiChnPutC(SPI_CHN,data);
}

unsigned char getByteSPI()
{
	return SpiChnGetC(SPI_CHN);
}

void spiOpenDrain (void)
{
	ODCB   = 0x000F;
	LATB   = 0x000F;
	TRISB &= 0xFFF0; 	
	ODCG   = 0x03C0;
	LATG   = 0x03C0;
	TRISG &= 0xFC3F;
}

