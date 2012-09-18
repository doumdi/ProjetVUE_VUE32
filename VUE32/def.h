
/*def.h*/
#ifndef DEF_H
#define DEF_H
/*General Include for any PIC32*/
#include <plib.h>
#include <p32xxxx.h>


#define VUE32_1     0x01
#define VUE32_2     0x02
#define VUE32_3     0x03
#define VUE32_4     0x04
#define VUE32_5a    0x05
#define VUE32_5b    0x06
#define VUE32_6     0x07
#define VUE32_7     0x08
#define VUE32_8     0x09
#define VUE32_9     0x0A
#define VUE32_10    0x0B
#define MAX_FUNCTION 20

/*Global define*/
#define ADC_ON_BOARD


/*System Clock definition and Peripheral Clock*/
#define SYSCLK  80000000
#define FPB     SYSCLK

/*Global Variable usable by anybody*/
void (*function_table[MAX_FUNCTION]) ();
unsigned char ucFunctTableSize; //counter that keep count of the number of function in the function_table
unsigned int (*variable_array[MAX_FUNCTION]);
unsigned char ucVariableArraySize;//counter that keep count of the number of function in the function_table
unsigned int unGlobalTimestamp; //Generated by Timer 1, serve as a reference for everyone
unsigned char power_module;
unsigned Flag_Timer1;


    
unsigned char cLeftLight;//Left Light activation variable
unsigned char cRightLight;//Right Light activation variable
unsigned char cLowLight;//Low Light activation variable
unsigned char cHighLight; //High Light activation variable
unsigned char cNightLight;//Night Light activation variable
unsigned char cBrakeLight;//Brake Light activation Variable
unsigned char cCoolingPump;//Cooling Pump activation Variable
unsigned char cWiperFrontPump;// Wiper pump activation Variable
unsigned char cWiperBackPump;// Wiper pump activation Variable
unsigned char cCharger;//Charger Indicator Variable

unsigned int speed[2];

/*Section of the different definition for Board VUE32*/
#if defined ADC_ON_BOARD
    /*ADC 1 configuration (Change the channel or the Sample buffer, but don't forget that data come at 8kHz)*/

        #define AD1_NUM_CH 		7   //how many do we scan?

    /*conversion of the current sensor*/
        #define CONV_CURRENT(x) (x*3) // mA/bits

    /*conversion of the TC1047A*/

        #define TEMP_OFFSET 155 //bits (500 mV * 310.303(bits/V))^-1
        #define CONV_TEMP(x) ((x-TEMP_OFFSET)/3)  //bits/�C  ==> (310.303(bits/V) * 10(mV/�C))^-1

    /*conversion of the tension (in uV)*/
        #define CONV_VOLTAGE(x) ((x)*3223)//uV
    /*conversion of the accelerator foot*/
        #define ACCFT_CONST 2.5
        #define CONV_ACCFT(x) (x)*2.5;

        unsigned int AD1_mean[AD1_NUM_CH];
#endif

    unsigned int count;

    /** LED ************************************************************/
    #define mInitAllLEDs()          TRISBbits.TRISB12 = 0;LATBbits.LATB12 = 0;TRISBbits.TRISB13 = 0;LATBbits.LATB13 = 0;

    #define mLED_1              LATBbits.LATB12

    #define mLED_2              LATBbits.LATB13

#define MOTOR_SLOT_A 1
#define MOTOR_SLOT_B 2

#define COMPARE_TIMESTAMP(x,y) (( x + y < unGlobalTimestamp))
#define TIMESTAMP_SECURITY(x,y) if(unGlobalTimestamp<y)x=0


/* lock define */
#define LOCK_UNKNOWN 0
#define LOCK_UNLOCKED 1
#define LOCK_LOCKED 2


#endif










