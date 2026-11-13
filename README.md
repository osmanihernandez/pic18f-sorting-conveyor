# Automated Conveyor Sorting System

**PIC Microcontroller | Embedded C | MPLAB X | XC8**

A microcontroller-based conveyor prototype developed for an Embedded Systems Programming course at Florida International University. The system uses ultrasonic object detection to trigger a servo-driven sorting action while controlling conveyor movement through a relay-switched DC motor.

## Project Demonstration

https://github.com/user-attachments/assets/65fd9ee1-3424-4edf-a60b-181823ff22e9

> **Video preview:** GitHub cannot generate an embedded video preview from a Google Drive **folder** link. To display a playable video preview here, upload the MP4 directly to a GitHub issue or pull request, copy the resulting `github.com/user-attachments/assets/...` URL, and paste that URL on its own line below. GitHub will render the video player where supported.
>
> `PASTE_GITHUB_UPLOADED_VIDEO_URL_HERE`

## System overview

The PIC microcontroller coordinates three main functions:

1. **Conveyor drive:** A GPIO-controlled relay driver switches the DC motor on or off, using dedicated START and STOP buttons.
2. **Object detection:** The HC-SR04 ultrasonic sensor sends an ECHO pulse; a hardware timer measures its width to determine whether an object is within the detection threshold.
3. **Sorting actuation:** When an object is detected, the PIC issues timed control pulses to an SG90 servo, moving the sorting mechanism and then returning it to its initial position.

The current firmware uses a polling-based main loop and blocking delays for sequencing. It is a prototype of **sequential automation**, not a closed-loop motor-speed controller.

## Hardware

| Component                                                    | Function                                          |
| ------------------------------------------------------------ | ------------------------------------------------- |
| PIC18LF45K22 microcontroller on Explorer 8 development board | Embedded controller; GPIO and hardware timer      |
| HC-SR04 ultrasonic sensor                                    | Object detection via trigger and echo timing      |
| SG90 servo motor                                             | Sorting actuator                                  |
| DC conveyor motor                                            | Moves objects along the belt                      |
| Relay and transistor driver                                  | Switches motor power from a PIC GPIO signal       |
| START and STOP pushbuttons                                   | Manual conveyor control                           |
| Detection LED                                                | Visual indication of object detection             |
| External motor and servo power                               | Supplies actuator current separately from PIC I/O |
| Resistors and interconnect wiring                            | Input interfacing and circuit connections         |

### PIC pin assignments

| PIC signal | Connection           | Direction |
| ---------- | -------------------- | --------- |
| RA5        | START button         | Input     |
| RB0        | STOP button          | Input     |
| RB1        | HC-SR04 ECHO         | Input     |
| RD0        | HC-SR04 TRIG         | Output    |
| RD2        | Object detection LED | Output    |
| RC3        | Relay driver control | Output    |
| RC2        | SG90 servo signal    | Output    |

**Electrical interfacing:** The HC-SR04 ECHO output can reach 5 V; a resistor divider is used when interfacing with a lower-voltage PIC input. The servo and motor require suitable external supplies. Connect grounds together where required for the control signals, and do not power a motor directly from a PIC GPIO pin. Verify the actual supply voltage and relay-driver circuit before wiring.

## Software and firmware

- **Language:** Embedded C
- **IDE:** MPLAB X
- **Compiler:** Microchip XC8
- **Target:** PIC18LF45K22
- **Timing:** Hardware timer for ultrasonic echo measurements; software-generated servo control pulses
- **Architecture:** Modular firmware organized into application, device-driver, and system-configuration layers

### Firmware organization

```text
firmware/
└── Conveyor_Control.X/
    ├── main.c
    ├── config_bits.c
    ├── config/
    │   ├── system.c
    │   └── system.h
    ├── drivers/
    │   ├── dc_motor.c
    │   ├── dc_motor.h
    │   ├── ultrasonic.c
    │   ├── ultrasonic.h
    │   ├── servo.c
    │   └── servo.h
    └── app/
        ├── conveyor.c
        └── conveyor.h
```

_Folder layout is illustrative of the modular project organization; check the repository for the exact paths in the committed MPLAB X project._

### Control sequence

```text
Initialize PIC peripherals and servo
              |
              v
Check START / STOP inputs and control relay
              |
              v
Trigger HC-SR04 and measure ECHO pulse
              |
              v
        Object detected?
         /          \
       No           Yes
       |             |
       |      Wait for positioning
       |             |
       |       Move sorting servo
       |             |
       |       Return servo home
       |             |
       |      Wait until object clears
       |             |
       +-------------+
              |
              v
         Repeat loop
```

## Building and running

1. Open the MPLAB X project (`Conveyor_Control.X`) in MPLAB X IDE.
2. Select the **PIC18LF45K22** target device and the installed **XC8** compiler.
3. Verify oscillator configuration, pin assignments, and supply voltages against your hardware.
4. Build the firmware and program the controller with a compatible PIC programmer/debugger.
5. Connect the ultrasonic sensor, relay driver, servo, buttons, and power supplies according to the pin table.
6. Start the conveyor and verify detection and servo movement using the demonstration as a reference.

> **Safety:** Disconnect power before modifying wiring. Check the motor's current draw, relay contact rating, transistor driver, and flyback protection. Never connect a 5 V output directly to a 3.3 V-only input.

## Engineering concepts demonstrated

- Embedded C firmware development and modular driver design
- Microcontroller GPIO configuration and digital interfacing
- Hardware timer-based pulse measurement
- Relay-driven DC motor switching and actuator control
- Sensor/actuator integration, power distribution, and troubleshooting

## Scope and future improvements

The demonstrated prototype focuses on ultrasonic detection and servo-based sorting. Possible extensions include a TCS3200 color sensor for color classification, a nonblocking finite-state machine, improved fault handling, and quantitative sorting-accuracy testing. These are **future improvements**, not claims of completed functionality.
