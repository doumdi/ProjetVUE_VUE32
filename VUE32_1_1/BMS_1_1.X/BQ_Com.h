/** 
*
* @brief  Fichier de configuration du circuit imprimé BQ
*		  de TexasInstrument, Contient le nom de chacun des registres
*		  avec leur valeurs et les address de communication
* 
* @author Joel Viau
* 
* $Header $
*/

#ifndef BQ_COM_H
#define BQ_COM_H


//Address de broadcast
#define BROADCAST_ADDRESS 0x3F


//Description des registres

#define DEVICE_STATUS 	0x00	
#define GPAI			0x01	//length 2
#define VCELL1			0x03	//length 2
#define VCELL2			0x05	//length 2
#define VCELL3			0x07	//length 2
#define VCELL4			0x09	//length 2
#define VCELL5			0x0B	//length 2
#define VCELL6			0x0D	//length 2
#define TEMPERATURE1	0x0F	//length 2
#define TEMPERATURE2	0x11	//length 2
#define ALERT_STATUS	0x20	
#define FAULT_STATUS	0x21	
#define COV_FAULT		0x22	
#define CUV_FAULT		0x23	
#define PRESULT_A		0x24	
#define PRESULT_B		0x25	
#define ADC_CONTROL		0x30	
#define IO_CONTROL		0x31	
#define CB_CTRL			0x32	
#define CB_TIME			0x33	
#define ADC_CONVERT		0x34	
#define SHDW_CTRL		0x3A	
#define ADDRESS_CONTROL	0x3B
#define RESET			0x3C
#define TEST_SELECT		0x3D
#define E_EN			0x3F
#define FUNCTION_CONFIG	0x40
#define IO_CONFIG		0x41
#define CONFIG_COV		0x42
#define CONFIG_COVT		0x43
#define CONFIG_CUV		0x44
#define CONFIG_CUVT		0x45
#define CONFIG_OT		0x46
#define CONFIG_OTT		0x47


typedef unsigned char byte;

/**
*Calcul du CRC lors de l'envoie d'une trame
* @param param1 explication1
* @param param2 explication2
* @return Explication de la valeur de retour.
*/

byte calculCRC(byte buffer[], int length);

//Écriture dans un registe de BQ
//cs: chip select
//device: addresse du BQ
//reg: addresse du registre
//data: byte à envoyer
void writeRegister(byte cs, byte device, byte reg, byte data);

//Lecture dans un registre de BQ
//cs: chip select
//device: addresse du BQ
//startreg: addresse du registre de départ 
//length: longueur du buffer à envoyer
//data: pointeur vers le buffer à envoyer
char readRegister(byte cs, byte device, byte startReg, byte length, byte *data);

#endif //BQ_COM_H
