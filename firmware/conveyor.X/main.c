
#include <xc.h>

#include "config/system.h"
#include "app/conveyor.h"
#include "drivers/ultrasonic.h"

void main(void)
{
    // Initialize hardware
    System_Init();

    while(1)
    {
        // Handle motor START/STOP buttons
        Conveyor_Task();
        
        // Measure distance and update LED
        Ultrasonic_Update();
        
        // Pause to inspect variables
        //__debug_break();

    }
}
