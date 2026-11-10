
#include <xc.h>
#include "ultrasonic.h"

#define _XTAL_FREQ 8000000

volatile unsigned int echo_start = 0;
volatile unsigned int echo_end = 0;
volatile unsigned int echo_time = 0;

volatile unsigned char echo_detected = 0;
volatile unsigned char measurement_valid = 0;
volatile unsigned char object_detected = 0;

void Ultrasonic_Update(void)
{
    echo_start = 0;
    echo_end = 0;
    echo_time = 0;
    echo_detected = 0;
    measurement_valid = 0;
    object_detected = 0;

    // Stop and reset Timer1
    T1CONbits.TMR1ON = 0;
    TMR1H = 0;
    TMR1L = 0;
    PIR1bits.TMR1IF = 0;

    if(PORTBbits.RB1 == 0)
    {
        // Generate TRIG pulse
        LATDbits.LATD0 = 0;
        __delay_us(2);

        LATDbits.LATD0 = 1;
        __delay_us(10);
        LATDbits.LATD0 = 0;

        // Start Timer1
        T1CONbits.TMR1ON = 1;

        // Wait for ECHO HIGH
        while(PORTBbits.RB1 == 0 &&
              PIR1bits.TMR1IF == 0)
        {
        }

        if(PIR1bits.TMR1IF == 0)
        {
            echo_detected = 1;

            echo_start =
                ((unsigned int)TMR1H << 8) | TMR1L;

            // Wait for ECHO LOW
            while(PORTBbits.RB1 == 1 &&
                  PIR1bits.TMR1IF == 0)
            {
            }

            if(PIR1bits.TMR1IF == 0)
            {
                echo_end =  
                    ((unsigned int)TMR1H << 8) | TMR1L;

                echo_time = echo_end - echo_start;

                measurement_valid = 1;

                // Detect object within ~15 cm
                if(echo_time > 100 &&
                   echo_time < 870)
                {
                    object_detected = 1;
                }
            }
        }
    }

    // Stop Timer1
    T1CONbits.TMR1ON = 0;

    // Update detection LED
    LATDbits.LATD2 = object_detected;

    // Wait before next measurement
         __delay_ms(60);
}
