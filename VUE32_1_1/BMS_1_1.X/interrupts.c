#include "def.h"

//"The user-selectable priority levels range from 1 (lowest) to 7 (highest)."
// TODO: Move this file to the "src" folder

volatile unsigned int uiTimeStamp = 0;

//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
//                                         Interruptions                                    //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

//Timer 1 - Main timebase 100µs - 10kHz
void __ISR(_TIMER_1_VECTOR, ipl3) isr_timer1(void)
{
    static unsigned int led_cnt = 0;

    //125ms - 8Hz (heart beat)
    led_cnt++;
    if(led_cnt > 1250)
    {
        led_cnt = 0;
        LED1 ^= 1;  //Toggle LED 4Hz
    }

    //Clear flag and return
    IFS0bits.T1IF = 0;

    //Local Timestamp uses by the long polling functionnality
    uiTimeStamp++;

}

//Timer 2
void __ISR(_TIMER_2_VECTOR, ipl1) isr_timer2(void)
{
    //Shouldn't happen, error

    IFS0bits.T2IF = 0;           // Clear interrupt flag
}

//Timer 3 - PWM - 20kHz
void __ISR(_TIMER_3_VECTOR, ipl3) isr_timer3 (void)
{
    //Should not happen...
    
    IFS0bits.T3IF = 0;           // Clear interrupt flag
}

//Timer 4
void __ISR(_TIMER_4_VECTOR, ipl1) isr_timer4(void)
{
    //Shouldn't happen, error

    IFS0bits.T4IF = 0;           // Clear interrupt flag
}

//Timer 5 - Custom PWM for output 4 - Wheel sensors
void __ISR(_TIMER_5_VECTOR, ipl4) isr_timer5(void)
{
    // PWM?

    IFS0bits.T5IF = 0;           // Clear interrupt flag
}
