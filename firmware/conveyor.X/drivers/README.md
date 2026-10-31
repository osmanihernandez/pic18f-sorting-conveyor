\# Hardware Drivers



\## Overview



This directory contains the hardware drivers for the PIC18F4520 automated color-sorting conveyor.



A \*\*driver\*\* is a C module responsible for communicating with and controlling a specific hardware component.



Each driver should provide functions that other parts of the firmware can call without needing to know the details of the microcontroller registers or electrical signals.



For example, the sorting application should be able to call `motor\_start()` instead of directly modifying a PIC18F4520 output register.



\## Planned Files and Responsibilities



| Files | Hardware | Responsibility |

|---|---|---|

| `motor.c`, `motor.h` | Conveyor motor and motor driver | Start, stop, and optionally control conveyor speed |

| `servo.c`, `servo.h` | Sorting servo | Control the sorting gate position |

| `ultrasonic.c`, `ultrasonic.h` | HC-SR04 | Detect blocks and measure distance |

| `color\_sensor.c`, `color\_sensor.h` | TCS3200 (planned) | Measure reflected light and classify block colors |



The final motor driver, servo model, and color sensor must be confirmed before implementing hardware-specific code.



\## 1. Creating a New Driver



Every hardware driver should generally have two files:



\- `.h` — Public interface, containing function declarations, types, and constants.

\- `.c` — Implementation, containing the actual code that communicates with the hardware.



For example, when implementing the conveyor motor, create:



```text

drivers/

├── README.md

├── motor.h

└── motor.c

```



\### Step A — Write the header file



\*\*`motor.h`\*\*



```c

\#ifndef MOTOR\_H

\#define MOTOR\_H



void motor\_init(void);

void motor\_start(void);

void motor\_stop(void);



\#endif

```



The header declares three functions that other modules can use.



Include guards prevent the header's contents from being processed repeatedly within the same translation unit.



\### Step B — Write the implementation



\*\*`motor.c`\*\*



```c

\#include <xc.h>

\#include "motor.h"

\#include "../config/pins.h"



void motor\_init(void)

{

&#x20;   // Configure motor control pins

&#x20;   // Set initial motor state to OFF

}



void motor\_start(void)

{

&#x20;   // Enable the motor driver

}



void motor\_stop(void)

{

&#x20;   // Disable the motor driver

}

```



The functions above are templates. They do not yet control a physical motor.



The actual implementation depends on the motor driver, wiring, and electrical requirements.



\### Step C — Use the driver from `main.c`



Because `main.c` is located in the parent directory of `drivers/`, include the header using:



```c

\#include <xc.h>

\#include "drivers/motor.h"



int main(void)

{

&#x20;   motor\_init();



&#x20;   while (1)

&#x20;   {

&#x20;       // Sorting application will control the motor

&#x20;   }



&#x20;   return 0;

}

```



\*\*Important:\*\* Including `motor.h` makes the declarations available to `main.c`. It does not automatically compile `motor.c`.



Both `main.c` and `motor.c` must be included in the MPLAB X project.



Never include `.c` files directly into other `.c` files.



\## 2. Planned Driver Interfaces



The following are proposed interfaces to help team members understand what their modules should provide. They can be adjusted as the hardware design develops.



\### Motor Driver — `motor.h`



```c

\#ifndef MOTOR\_H

\#define MOTOR\_H



void motor\_init(void);

void motor\_start(void);

void motor\_stop(void);



\#endif

```



\*\*Hardware responsibility:\*\* Control the conveyor motor through an appropriate motor driver.



The PIC18F4520 must not power the conveyor motor directly from its GPIO pins.



\### Servo Driver — `servo.h`



```c

\#ifndef SERVO\_H

\#define SERVO\_H



\#include <stdint.h>



void servo\_init(void);

void servo\_set\_position(uint8\_t position);

void servo\_home(void);



\#endif

```



\*\*Hardware responsibility:\*\* Generate the control pulses required to position the sorting gate.



The meaning and valid range of `position` must be defined when implementing the driver.



\### Ultrasonic Driver — `ultrasonic.h`



```c

\#ifndef ULTRASONIC\_H

\#define ULTRASONIC\_H



\#include <stdint.h>

\#include <stdbool.h>



void ultrasonic\_init(void);

bool ultrasonic\_read\_distance\_cm(uint16\_t \*distance\_cm);



\#endif

```



\*\*Hardware responsibility:\*\* Trigger the HC-SR04 and measure the returning echo pulse.



The proposed function returns `true` when a valid measurement is available and `false` when the measurement fails or times out.



\### Color Sensor Driver — `color\_sensor.h`



```c

\#ifndef COLOR\_SENSOR\_H

\#define COLOR\_SENSOR\_H



typedef enum {

&#x20;   COLOR\_UNKNOWN,

&#x20;   COLOR\_RED,

&#x20;   COLOR\_BLUE

} Color;



void color\_sensor\_init(void);

Color color\_sensor\_read(void);



\#endif

```



\*\*Hardware responsibility:\*\* Measure the color sensor's output and classify the detected block.



For a TCS3200, the implementation will select the sensor's color filters and measure output frequency using PIC18F4520 timer or counter peripherals.



If the team selects a different color sensor, this driver can be adapted without changing the rest of the application interface.



\## 3. How Drivers Connect to the Sorting Application



```text

&#x20;                  main.c

&#x20;                     |

&#x20;                     v

&#x20;               app/sorting.c

&#x20;                     |

&#x20;        +------------+------------+

&#x20;        |            |            |

&#x20;        v            v            v

&#x20;     motor.c     ultrasonic.c  color\_sensor.c

&#x20;        |            |            |

&#x20;        v            v            v

&#x20;   Motor Driver    HC-SR04      Color Sensor



&#x20;               app/sorting.c

&#x20;                     |

&#x20;                     v

&#x20;                  servo.c

&#x20;                     |

&#x20;                     v

&#x20;               Sorting Servo

```



The application decides \*\*when\*\* an action should happen.



The driver determines \*\*how\*\* to perform that action using the microcontroller hardware.



For example, `sorting.c` may call `motor\_stop()`, but `motor.c` is responsible for producing the correct electrical signal.



\## 4. PIC18F4520 Hardware Resources



Drivers may use the following microcontroller features:



| Feature | Possible Application |

|---|---|

| GPIO | Motor enable, sensor trigger, sensor selection |

| Timer0 / Timer1 | Timing and pulse measurements |

| CCP modules | Capture, compare, or PWM operations |

| Interrupts | Detect time-sensitive events |

| ADC | Optional analog sensors |



\*\*Important:\*\* PIC18F4520 peripherals are shared resources.



Team members must coordinate timer, interrupt, CCP, and pin assignments to avoid conflicts between drivers.



For example, the ultrasonic driver and color sensor driver should not independently reconfigure the same timer without an agreed design.



\## 5. Adding Drivers to MPLAB X



After creating a new driver:



1\. Open the project in MPLAB X.

2\. Switch to the \*\*Projects\*\* tab.

3\. Right-click \*\*Source Files\*\* and select \*\*Add Existing Item\*\*.

4\. Add the driver's `.c` file.

5\. Right-click \*\*Header Files\*\* and add its `.h` file for convenient navigation.

6\. Build the project to check for compilation errors.



Adding a header to the Projects tab is optional for compilation, but its include path must resolve correctly.



Do not manually modify MPLAB-generated Makefiles.



\## 6. Development Guidelines



\- Each hardware component should have its own driver.

\- Keep hardware-specific register operations inside the appropriate driver.

\- Declare public functions in `.h` files and implement them in `.c` files.

\- Include headers using `#include`, not source files.

\- Keep shared pin definitions in `config/pins.h`.

\- Avoid long blocking delays when practical.

\- Implement timeouts for sensor operations that may fail.

\- Keep interrupt service routines short.

\- Coordinate shared hardware peripherals before implementation.

\- Test each driver independently before integrating it with the sorting application.

\- Document any changes to public driver interfaces.



\## 7. Suggested Team Workflow



Each team member can work on a separate feature branch.



Examples:



```bash

git switch main

git pull origin main

git switch -c feature/motor-control

```



After implementing and testing the driver:



```bash

git add .

git commit -m "Implement conveyor motor driver"

git push -u origin feature/motor-control

```



Create a pull request on GitHub to merge the feature into `main`.



Avoid editing unrelated modules on the same branch unless necessary.



\## Current Status



The driver interfaces above are proposed starting points. No working hardware drivers have been implemented yet.



The initial goal is to support one-block-at-a-time sorting of red and blue blocks using the PIC18F4520.

