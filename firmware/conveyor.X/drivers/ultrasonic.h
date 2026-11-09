
#ifndef ULTRASONIC_H
#define ULTRASONIC_H

extern volatile unsigned int echo_start;
extern volatile unsigned int echo_end;
extern volatile unsigned int echo_time;

extern volatile unsigned char echo_detected;
extern volatile unsigned char measurement_valid;
extern volatile unsigned char object_detected;

void Ultrasonic_Update(void);

#endif
