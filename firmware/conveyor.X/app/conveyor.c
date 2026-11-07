#include <xc.h>
#include "conveyor.h"
#include "../drivers/dc_motor.h"

void Conveyor_Task(void)
{
    // S2 pressed: turn motor ON
    if(PORTAbits.RA5 == 0)
    {
        Motor_On();
    }

    // S1 pressed: turn motor OFF
    if(PORTBbits.RB0 == 0)
    {
        Motor_Off();
    }
}
