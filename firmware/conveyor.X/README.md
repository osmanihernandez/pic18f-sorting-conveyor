\# PIC18F4520 Automated Color-Sorting Conveyor



An embedded systems project that uses a \*\*PIC18F4520 microcontroller\*\* to automate the detection, identification, and sorting of colored objects on a miniature conveyor belt.



Developed for \*\*EEL 4730 — Programming Embedded Systems\*\* at Florida International University.



\## Project Overview



The goal is to design and build a small automated conveyor system capable of detecting objects, identifying their colors, and directing them into separate collection bins.



The system integrates embedded C programming, digital sensors, motor control, timer peripherals, and a servo-operated sorting mechanism.



The PIC18F4520 serves as the central controller, coordinating all sensors and actuators through a finite-state machine.



The initial system will sort \*\*red and blue objects\*\*, processing one object at a time.



\## System Architecture



```text

&#x20;                 AUTOMATED COLOR-SORTING CONVEYOR



&#x20;                        +--------------------+

&#x20;                        |    Power Supply    |

&#x20;                        |  Regulated Rails   |

&#x20;                        +--------------------+

&#x20;                                  |

&#x20;                 +----------------+----------------+

&#x20;                 |                                 |

&#x20;                 v                                 v

&#x20;       +-------------------+             +-------------------+

&#x20;       |    PIC18F4520     |             | Motor/Servo Power |

&#x20;       |  Microcontroller  |             +-------------------+

&#x20;       +-------------------+

&#x20;          ^           ^

&#x20;          |           |

&#x20;  +-------+---+  +----+---------+

&#x20;  | HC-SR04   |  | TCS3200      |

&#x20;  | Ultrasonic|  | Color Sensor |

&#x20;  | Sensor    |  | (Planned)    |

&#x20;  +-----------+  +--------------+

&#x20;          |

&#x20;          |     PIC18F4520 CONTROL OUTPUTS

&#x20;          |

&#x20;          +------------+------------------+

&#x20;                       |                  |

&#x20;                       v                  v

&#x20;               +---------------+   +--------------+

&#x20;               | Motor Driver  |   | Servo Motor  |

&#x20;               +---------------+   +--------------+

&#x20;                       |                  |

&#x20;                       v                  v

&#x20;               +---------------+   +--------------+

&#x20;               | Conveyor      |   | Sorting Gate |

&#x20;               | DC Motor      |   +--------------+

&#x20;               +---------------+

```



The sensors provide input signals to the PIC18F4520. The microcontroller processes those signals and sends commands to the motor driver and servo.



The motor driver supplies the power required by the conveyor motor. The servo positions the sorting gate to direct objects into their corresponding bins.



All interconnected control signals must use compatible voltage levels, and the system must have an appropriate common ground where required.



\## How the System Works



1\. \*\*Initialize:\*\* The PIC18F4520 initializes the sensors, motor driver, servo, and required peripherals.

2\. \*\*Transport:\*\* The conveyor moves objects toward the detection area.

3\. \*\*Detect:\*\* The HC-SR04 ultrasonic sensor detects an approaching object.

4\. \*\*Identify:\*\* The conveyor stops at the measurement position, and the color sensor identifies the object as red, blue, or unknown.

5\. \*\*Position:\*\* The conveyor moves the identified object toward the sorting mechanism.

6\. \*\*Sort:\*\* The servo moves the sorting gate to direct the object into the appropriate bin.

7\. \*\*Reset:\*\* The sorting gate returns to its initial position, and the system prepares for the next object.



The final object-positioning method and timing will be determined through hardware testing.



\## Hardware Components



| Component | Function | Status |

|---|---|---|

| PIC18F4520 | Main embedded controller | Selected |

| PICkit 3 | Programming and debugging | Selected |

| HC-SR04 | Object detection | Planned |

| TCS3200 | Color identification | Planned |

| DC motor and conveyor | Transport objects | Specifications pending |

| Motor driver | Control and power the conveyor motor | Selection pending |

| Servo motor | Operate the sorting gate | Selection pending |

| Regulated power supply | Power the controller, sensors, and actuators | Design pending |



The hardware design, pin assignments, and voltage requirements must be verified before final integration.



\## Firmware Architecture



The firmware is organized into three main layers:



```text

+------------------------------------------------+

|                  main.c                        |

|         System entry point and main loop       |

+------------------------------------------------+

&#x20;                       |

&#x20;                       v

+------------------------------------------------+

|                Application Layer               |

|                 app/sorting.c                  |

|        State machine and sorting logic         |

+------------------------------------------------+

&#x20;                       |

&#x20;                       v

+------------------------------------------------+

|                  Driver Layer                  |

|                                                |

|  motor.c    ultrasonic.c    color\_sensor.c     |

|                  servo.c                       |

+------------------------------------------------+

&#x20;                       |

&#x20;                       v

+------------------------------------------------+

|            PIC18F4520 Hardware                 |

|       GPIO, Timers, CCP, Interrupts            |

+------------------------------------------------+



&#x20;      config/ contains shared configuration

&#x20;               and pin definitions

```



\### Application Layer



The application coordinates the complete sorting process.



It decides when to start and stop the conveyor, read sensors, move the sorting gate, and transition between operating states.



\### Driver Layer



Each hardware component has a dedicated driver.



Drivers provide functions such as `motor\_start()`, `motor\_stop()`, and `color\_sensor\_read()` so that the application does not need to manipulate hardware registers directly.



\### Configuration



The configuration directory centralizes microcontroller settings and pin definitions to keep the hardware interfaces consistent.



\## Proposed State Machine



```text

IDLE

&#x20; |

&#x20; v

WAIT\_FOR\_BLOCK

&#x20; |

&#x20; | Object detected

&#x20; v

MEASURE\_COLOR

&#x20; |

&#x20; | Valid color

&#x20; v

MOVE\_TO\_SORTER

&#x20; |

&#x20; | Object positioned

&#x20; v

SORT

&#x20; |

&#x20; v

RESET

&#x20; |

&#x20; +----------> WAIT\_FOR\_BLOCK



ERROR: Entered when an operation cannot

&#x20;      be completed safely or reliably.

```



The application will implement the states in `app/sorting.c`.



See `firmware/conveyor.X/app/README.md` for the planned application interface and programming examples.



\## Repository Structure



The following tree shows the intended organization. Some source files and documents are \*\*planned and have not yet been created\*\*.



```text

pic18f-sorting-conveyor/

|

|-- firmware/

|   `-- conveyor.X/

|       |-- main.c

|       |-- Makefile

|       |-- nbproject/

|       |

|       |-- config/

|       |   |-- README.md

|       |   |-- pins.h             \[planned]

|       |   `-- config\_bits.c      \[planned]

|       |

|       |-- drivers/

|       |   |-- README.md

|       |   |-- motor.c/.h         \[planned]

|       |   |-- servo.c/.h         \[planned]

|       |   |-- ultrasonic.c/.h    \[planned]

|       |   `-- color\_sensor.c/.h  \[planned]

|       |

|       `-- app/

|           |-- README.md

|           `-- sorting.c/.h       \[planned]

|

|-- hardware/

|   |-- README.md                 \[planned]

|   |-- components.md             \[planned]

|   |-- pinout.md                 \[planned]

|   `-- schematics/

|       `-- README.md             \[planned]

|

|-- docs/

|   `-- README.md                 \[planned]

|

|-- .gitignore

`-- README.md

```



\## Getting Started



\### Development Tools



\- MPLAB X IDE

\- Microchip XC8 compiler v3.10

\- PIC18F4520 device support pack

\- PICkit 3 programmer/debugger

\- Git and GitHub



\### Clone the Repository



```bash

git clone https://github.com/YOUR\_USERNAME/pic18f-sorting-conveyor.git

cd pic18f-sorting-conveyor

```



Replace `YOUR\_USERNAME` with the actual repository owner.



\### Open the MPLAB X Project



1\. Launch MPLAB X IDE.

2\. Select \*\*File → Open Project\*\*.

3\. Navigate to `firmware/conveyor.X`.

4\. Open the project.

5\. Verify that PIC18F4520 is selected as the target device.

6\. Verify the XC8 compiler and hardware tool configuration.

7\. Build the project.



\### Adding New C Modules



When implementing a new hardware driver:



1\. Create the `.h` and `.c` files inside the appropriate physical directory.

2\. Declare public functions in the header.

3\. Implement the functions in the source file.

4\. Include the header in any source file that needs those functions.

5\. Add the new `.c` file to MPLAB X under \*\*Source Files\*\*.

6\. Build and test the module.



For example:



```c

// In main.c

\#include "drivers/motor.h"



int main(void)

{

&#x20;   motor\_init();



&#x20;   while (1)

&#x20;   {

&#x20;       // Application loop

&#x20;   }



&#x20;   return 0;

}

```



The corresponding `motor.c` file must be included in the MPLAB X build.



Refer to the README inside each firmware directory for detailed code templates and integration instructions.



\## Team Collaboration



The repository is organized so team members can develop individual components independently while following consistent interfaces.



\### Recommended Feature Branches



| Branch | Responsibility |

|---|---|

| `feature/motor-control` | Conveyor motor driver |

| `feature/ultrasonic-sensor` | Object detection |

| `feature/color-detection` | Color sensor integration |

| `feature/servo-control` | Sorting gate control |

| `feature/sorting-logic` | Application state machine |

| `feature/hardware-design` | Wiring and hardware documentation |



\### Development Workflow



```bash

git switch main

git pull origin main

git switch -c feature/your-feature

```



After implementing and testing a feature:



```bash

git add .

git commit -m "Describe the implemented feature"

git push -u origin feature/your-feature

```



Open a pull request on GitHub for review before merging into `main`.



\### Collaboration Guidelines



\- Read the README in the relevant directory before creating files.

\- Create source files only when implementation begins.

\- Keep hardware-specific logic inside drivers.

\- Avoid editing another teammate's module without coordination.

\- Agree on pin assignments and timer usage before integrating hardware.

\- Add new `.c` files to the MPLAB X project configuration.

\- Test changes before submitting pull requests.

\- Update documentation when hardware or software interfaces change.

\- Keep generated build outputs out of version control.



\## Testing and Integration



The system will be developed and verified incrementally:



1\. Verify microcontroller configuration and basic GPIO operation.

2\. Test the conveyor motor driver independently.

3\. Verify ultrasonic distance measurements.

4\. Calibrate the color sensor using red and blue objects.

5\. Test servo positioning and sorting gate movement.

6\. Integrate the drivers into the application state machine.

7\. Perform repeated end-to-end sorting tests.

8\. Record system behavior, limitations, and results.



\## Project Goals



The final demonstration aims to show:



\- Reliable detection of objects on the conveyor.

\- Classification of red and blue objects.

\- Coordinated control of the conveyor motor and sorting servo.

\- Use of PIC18F4520 peripherals for sensor timing and actuator control.

\- Modular embedded C firmware with documented interfaces.

\- A functional automated sorting demonstration.



\## Current Development Status



\*\*Phase: Project architecture and repository setup\*\*



The MPLAB X project has been initialized, and the firmware structure is being documented. Hardware integration, driver implementation, and sorting application development are upcoming tasks.



The design may evolve as the team selects and tests the physical components.

