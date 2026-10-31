\# Microcontroller Configuration



\## Overview



This directory contains the hardware configuration for the \*\*PIC18F4520\*\* microcontroller used in the automated color-sorting conveyor.



The configuration files define how the microcontroller operates and which pins connect to external hardware.



\*\*Development environment\*\*

\- Microcontroller: PIC18F4520

\- IDE: MPLAB X

\- Compiler: XC8 v3.10

\- Programmer/debugger: PICkit 3

\- Language: Embedded C



\## Planned Files



| File | Purpose |

|---|---|

| `config\_bits.c` | Configure oscillator selection, watchdog timer, and other PIC18F4520 configuration bits |

| `pins.h` | Define hardware pin assignments and symbolic names used throughout the project |



These files should be created when the corresponding configuration work begins.



\## 1. Creating `pins.h`



Create `pins.h` inside this directory.



Header files contain definitions and declarations that can be shared between C source files.



Example:



```c

\#ifndef PINS\_H

\#define PINS\_H



\#include <xc.h>



// Example pin assignments only.

// Confirm actual wiring before using these definitions.



\#define MOTOR\_ENABLE\_LAT  LATCbits.LATC0

\#define MOTOR\_ENABLE\_TRIS TRISCbits.TRISC0



\#endif

```



The `#ifndef`, `#define`, and `#endif` statements form an \*\*include guard\*\*. They prevent the header's contents from being processed multiple times in the same compilation unit.



The example uses `RC0` for a motor driver enable signal. This is not a finalized hardware pin assignment.



\## 2. Including the Header in a C File



To use definitions from `pins.h`, include the header in the source file.



For example, from `main.c`, which is located in the parent directory:



```c

\#include <xc.h>

\#include "config/pins.h"



int main(void)

{

&#x20;   MOTOR\_ENABLE\_TRIS = 0;  // Configure pin as output

&#x20;   MOTOR\_ENABLE\_LAT = 0;   // Start with motor disabled



&#x20;   while (1)

&#x20;   {

&#x20;       // Main application loop

&#x20;   }



&#x20;   return 0;

}

```



The path `"config/pins.h"` is relative to the location of `main.c`.



If another source file is located in `drivers/`, it can use:



```c

\#include "../config/pins.h"

```



Alternatively, configure compiler include directories in MPLAB X and use a consistent include path across modules.



\## 3. Creating `config\_bits.c`



This file will contain the PIC18F4520 configuration-bit directives.



Example syntax:



```c

\#include <xc.h>



// Example only:

// #pragma config WDT = OFF

```



Configuration settings must be selected based on the actual microcontroller, oscillator circuit, and project requirements.



Use the MPLAB X Configuration Bits window to generate the appropriate directives.



Avoid defining conflicting configuration bits in multiple source files.



Unlike a reusable driver, `config\_bits.c` normally does not require its own header because it does not expose application functions.



\## 4. How C and Header Files Work Together



A header file (`.h`) typically provides declarations, macros, constants, and shared types.



A source file (`.c`) contains the implementation of functions.



For example:



\*\*motor.h\*\*



```c

\#ifndef MOTOR\_H

\#define MOTOR\_H



void motor\_init(void);

void motor\_start(void);

void motor\_stop(void);



\#endif

```



\*\*motor.c\*\*



```c

\#include "motor.h"

\#include "../config/pins.h"



void motor\_init(void)

{

&#x20;   MOTOR\_ENABLE\_TRIS = 0;

&#x20;   MOTOR\_ENABLE\_LAT = 0;

}



void motor\_start(void)

{

&#x20;   MOTOR\_ENABLE\_LAT = 1;

}



void motor\_stop(void)

{

&#x20;   MOTOR\_ENABLE\_LAT = 0;

}

```



\*\*main.c\*\*



```c

\#include <xc.h>

\#include "drivers/motor.h"



int main(void)

{

&#x20;   motor\_init();



&#x20;   while (1)

&#x20;   {

&#x20;       // Application logic

&#x20;   }



&#x20;   return 0;

}

```



The header makes the functions visible to other source files. The implementation in `motor.c` must also be compiled and linked into the MPLAB X project.



These examples illustrate program organization. They do not yet implement a complete or tested conveyor motor driver.



\## 5. Adding Files to MPLAB X



After creating a new file:



1\. Open the project in MPLAB X.

2\. Switch to the \*\*Projects\*\* tab.

3\. Right-click \*\*Source Files\*\* or \*\*Header Files\*\*, depending on the file type.

4\. Select \*\*Add Existing Item\*\*.

5\. Navigate to the file's physical location.

6\. Add it to the project.

7\. Build the project to verify that the code compiles.



Do not manually modify MPLAB-generated Makefiles just to add a source file.



\## 6. Development Guidelines



\- Use the XC8 compiler and PIC18F4520 device definitions.

\- Keep pin assignments centralized in `pins.h`.

\- Use descriptive names instead of hardcoded pin references throughout the application.

\- Document changes to pin assignments in `hardware/pinout.md`.

\- Coordinate shared pin assignments before changing them.

\- Avoid putting executable application logic directly into header files.

\- Test configuration changes before merging them into the `main` branch.



\## Current Status



The MPLAB X project has been created. The final pin assignments, oscillator configuration, and hardware interfaces are still being determined.



\*\*This README contains starting examples, not finalized hardware firmware.\*\*

