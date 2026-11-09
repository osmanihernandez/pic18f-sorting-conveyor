#include <xc.h>
#include "system.h"

void System_Init(void)
{
    // Set relay output LOW before enabling it
    LATCbits.LATC3 = 0;

    // Configure digital I/O
    ANSELA = 0x00;
    ANSELB = 0x00;
    ANSELC = 0x00;

    // Configure buttons as inputs
    TRISAbits.TRISA5 = 1;  // S2 - ON
    TRISBbits.TRISB0 = 1;  // S1 - OFF

    // Configure relay as output
    TRISCbits.TRISC3 = 0;
    
    
    // Ultrasonic TRIG
    LATDbits.LATD0 = 0;
    TRISDbits.TRISD0 = 0;

    // Ultrasonic ECHO
    ANSELBbits.ANSB1 = 0;
    TRISBbits.TRISB1 = 1;

    // Object detection LED
    LATDbits.LATD2 = 0;
    TRISDbits.TRISD2 = 0;

    // Timer1: 1 us per count at 8 MHz
    T1CON = 0x10;

}
