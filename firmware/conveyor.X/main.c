
#include <xc.h>
#include "config/system.h"
#include "app/conveyor.h"
#include "drivers/ultrasonic.h"
#include "drivers/servo.h"

#define _XTAL_FREQ 8000000

void main(void)
{
    System_Init();
    Servo_Init();

    while (1)
    {
        Conveyor_Task();
        Ultrasonic_Update();

        if (object_detected == 1)
        {
            __delay_ms(1650);

            Servo_Move(0);

            __delay_ms(1000);

            Servo_Move(1);

            do
            {
                Ultrasonic_Update();
            }
            while (object_detected == 1);
        }
    }
}
