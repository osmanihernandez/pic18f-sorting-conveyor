
#include <xc.h>

#include "config/system.h"
#include "app/conveyor.h"

void main(void)
{
    // Initialize microcontroller hardware
    System_Init();

    // Main program loop
    while(1)
    {
        Conveyor_Task();
    }
}




//#include <xc.h>
//
//#define _XTAL_FREQ 8000000
//
//void main(void)
//{
//    // Configure button pins as digital inputs
//    ANSELA = 0x00;
//    ANSELB = 0x00;
//    ANSELC = 0x00;
//
//    TRISAbits.TRISA5 = 1;  // S2 - Motor ON
//    TRISBbits.TRISB0 = 1;  // S1 - Motor OFF
//    TRISCbits.TRISC3 = 0;  // Relay output
//
//    LATCbits.LATC3 = 0;    // Motor initially OFF
//
//    while(1)
//    {
//        // Press S2 to turn motor ON
//        if(PORTAbits.RA5 == 0)
//        {
//            LATCbits.LATC3 = 1;
//        }
//
//        // Press S1 to turn motor OFF
//        if(PORTBbits.RB0 == 0)
//        {
//            LATCbits.LATC3 = 0;
//        }
//    }
//}



