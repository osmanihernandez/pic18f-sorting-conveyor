#include <xc.h>
#include "servo.h"

#define _XTAL_FREQ 8000000

void Servo_Init(void)
{
    TRISCbits.TRISC2 = 0;
    LATCbits.LATC2 = 0;

    Servo_Move(0);
}

void Servo_Move(unsigned char position)
{
    int i;

    for (i = 0; i < 100; i++)
    {
        LATCbits.LATC2 = 1;

        if (position == 0)
            __delay_us(750);
        else
            __delay_us(2250);

        LATCbits.LATC2 = 0;

        if (position == 0)
        {
            __delay_ms(19);
            __delay_us(250);
        }
        else
        {
            __delay_ms(17);
            __delay_us(750);
        }
    }
}
