#include <xc.h>
#include "dc_motor.h"

void Motor_On(void)
{
    LATCbits.LATC3 = 1;
}

void Motor_Off(void)
{
    LATCbits.LATC3 = 0;
}
