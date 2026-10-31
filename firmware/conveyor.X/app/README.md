\# Application Logic



\## Overview



This directory contains the application-level logic for the PIC18F4520 automated color-sorting conveyor.



The application coordinates the hardware drivers and manages the overall sorting process using a \*\*finite-state machine (FSM)\*\*.



The application should determine \*\*what the system does and when\*\*, while the hardware drivers determine \*\*how each component operates\*\*.



\## Planned Files



| File | Responsibility |

|---|---|

| `sorting.h` | Declare the sorting application interface and system states |

| `sorting.c` | Implement the sorting state machine and coordinate hardware drivers |



These files will be created when application development begins.



\## 1. Understanding the Sorting State Machine



The initial implementation will use one-block-at-a-time sorting.



The proposed states are:



| State | Description |

|---|---|

| `SORT\_IDLE` | System is initialized and ready |

| `SORT\_WAIT\_FOR\_BLOCK` | Conveyor runs while waiting for a block |

| `SORT\_MEASURE\_COLOR` | Stop the conveyor and identify the block's color |

| `SORT\_MOVE\_TO\_SORTER` | Move the identified block toward the sorting gate |

| `SORT\_SORT` | Position the servo to direct the block into the correct bin |

| `SORT\_RESET` | Return the sorting gate to its initial position |

| `SORT\_ERROR` | Handle invalid measurements or system faults |



\### Proposed state transitions



```text

&#x20;      +------------------+

&#x20;      |    SORT\_IDLE     |

&#x20;      +------------------+

&#x20;               |

&#x20;               v

&#x20;      +------------------+

&#x20;      | WAIT\_FOR\_BLOCK   |<-------------+

&#x20;      +------------------+              |

&#x20;               |                        |

&#x20;        Block detected                  |

&#x20;               |                        |

&#x20;               v                        |

&#x20;      +------------------+              |

&#x20;      |  MEASURE\_COLOR   |              |

&#x20;      +------------------+              |

&#x20;               |                        |

&#x20;         Color detected                 |

&#x20;               |                        |

&#x20;               v                        |

&#x20;      +------------------+              |

&#x20;      | MOVE\_TO\_SORTER   |              |

&#x20;      +------------------+              |

&#x20;               |                        |

&#x20;        Block positioned                |

&#x20;               |                        |

&#x20;               v                        |

&#x20;      +------------------+              |

&#x20;      |       SORT       |              |

&#x20;      +------------------+              |

&#x20;               |                        |

&#x20;               v                        |

&#x20;      +------------------+              |

&#x20;      |      RESET       |--------------+

&#x20;      +------------------+



&#x20;      ERROR can be entered if

&#x20;      a measurement or operation fails.

```



\*\*Important:\*\* The physical mechanism for determining when a block reaches the sorting gate is not yet finalized. The team must determine whether to use timing, an additional sensor, or another positioning method.



\## 2. Creating `sorting.h`



The header file defines the public interface of the sorting application.



Create `sorting.h` inside `app/` when implementation begins.



Example:



```c

\#ifndef SORTING\_H

\#define SORTING\_H



typedef enum

{

&#x20;   SORT\_IDLE,

&#x20;   SORT\_WAIT\_FOR\_BLOCK,

&#x20;   SORT\_MEASURE\_COLOR,

&#x20;   SORT\_MOVE\_TO\_SORTER,

&#x20;   SORT\_SORT,

&#x20;   SORT\_RESET,

&#x20;   SORT\_ERROR

} SortingState;



void sorting\_init(void);

void sorting\_update(void);

SortingState sorting\_get\_state(void);



\#endif

```



\### Explanation



\- `SortingState` defines the possible operating states.

\- `sorting\_init()` initializes the application and its hardware drivers.

\- `sorting\_update()` executes the current state and checks whether the system should transition to another state.

\- `sorting\_get\_state()` returns the current state for debugging or monitoring.



The include guard prevents the header from being processed multiple times within the same compilation unit.



\## 3. Creating `sorting.c`



This file implements the sorting state machine.



Start by including the application header and the hardware driver headers.



\*\*`sorting.c`\*\*



```c

\#include <xc.h>

\#include <stdbool.h>

\#include <stdint.h>



\#include "sorting.h"



\#include "../drivers/motor.h"

\#include "../drivers/servo.h"

\#include "../drivers/ultrasonic.h"

\#include "../drivers/color\_sensor.h"



\#define BLOCK\_DETECTION\_DISTANCE\_CM 10U



static SortingState current\_state = SORT\_IDLE;

static Color detected\_color = COLOR\_UNKNOWN;



void sorting\_init(void)

{

&#x20;   motor\_init();

&#x20;   servo\_init();

&#x20;   ultrasonic\_init();

&#x20;   color\_sensor\_init();



&#x20;   motor\_stop();

&#x20;   servo\_home();



&#x20;   detected\_color = COLOR\_UNKNOWN;

&#x20;   current\_state = SORT\_IDLE;

}



SortingState sorting\_get\_state(void)

{

&#x20;   return current\_state;

}



void sorting\_update(void)

{

&#x20;   uint16\_t distance\_cm;



&#x20;   switch (current\_state)

&#x20;   {

&#x20;       case SORT\_IDLE:

&#x20;           current\_state = SORT\_WAIT\_FOR\_BLOCK;

&#x20;           break;



&#x20;       case SORT\_WAIT\_FOR\_BLOCK:

&#x20;           motor\_start();



&#x20;           if (ultrasonic\_read\_distance\_cm(\&distance\_cm))

&#x20;           {

&#x20;               if (distance\_cm <= BLOCK\_DETECTION\_DISTANCE\_CM)

&#x20;               {

&#x20;                   motor\_stop();

&#x20;                   current\_state = SORT\_MEASURE\_COLOR;

&#x20;               }

&#x20;           }

&#x20;           break;



&#x20;       case SORT\_MEASURE\_COLOR:

&#x20;           detected\_color = color\_sensor\_read();



&#x20;           if (detected\_color == COLOR\_UNKNOWN)

&#x20;           {

&#x20;               current\_state = SORT\_ERROR;

&#x20;           }

&#x20;           else

&#x20;           {

&#x20;               current\_state = SORT\_MOVE\_TO\_SORTER;

&#x20;           }

&#x20;           break;



&#x20;       case SORT\_MOVE\_TO\_SORTER:

&#x20;           // TODO: Move block to the sorting gate.

&#x20;           // Transition only when positioning is confirmed.

&#x20;           break;



&#x20;       case SORT\_SORT:

&#x20;           // TODO: Position the servo based on detected\_color.

&#x20;           // Wait for the sorting operation to complete.

&#x20;           break;



&#x20;       case SORT\_RESET:

&#x20;           // TODO: Return the servo to its home position.

&#x20;           // Confirm reset before accepting another block.

&#x20;           break;



&#x20;       case SORT\_ERROR:

&#x20;           motor\_stop();



&#x20;           // TODO: Implement error recovery.

&#x20;           break;



&#x20;       default:

&#x20;           motor\_stop();

&#x20;           current\_state = SORT\_ERROR;

&#x20;           break;

&#x20;   }

}

```



\### Important notes



This code is an \*\*initial application skeleton\*\*, not a complete working sorting program.



\- The 10 cm detection threshold is an example and must be calibrated.

\- The color measurement may require stabilization and multiple samples.

\- The movement, sorting, and reset states still need implementations.

\- A failed ultrasonic measurement currently leaves the application waiting; timeout and fault policies should be defined.

\- The state machine must not assume a block has reached the sorting gate without a reliable positioning method.



The `static` variables are private to `sorting.c`. Other modules should interact with the application through the functions declared in `sorting.h`.



\## 4. Connecting the Application to `main.c`



The project's existing `main.c` is located in:



```text

firmware/conveyor.X/main.c

```



Once `sorting.c` and `sorting.h` have been implemented, include the application header from `main.c`:



```c

\#include <xc.h>

\#include "app/sorting.h"



int main(void)

{

&#x20;   sorting\_init();



&#x20;   while (1)

&#x20;   {

&#x20;       sorting\_update();

&#x20;   }



&#x20;   return 0;

}

```



\### How this works



1\. The microcontroller starts executing `main()`.

2\. `sorting\_init()` initializes the sorting application and hardware drivers.

3\. The infinite `while (1)` loop repeatedly calls `sorting\_update()`.

4\. `sorting\_update()` executes the current state and determines when to change states.

5\. The application calls driver functions to interact with the hardware.



The main loop should remain simple. Hardware-specific code should stay inside the drivers, and sorting decisions should stay inside the application.



\## 5. How the Files Connect



```text

main.c

&#x20; |

&#x20; | #include "app/sorting.h"

&#x20; v

app/sorting.h

&#x20; |

&#x20; | Public declarations

&#x20; v

app/sorting.c

&#x20; |

&#x20; +-- #include "../drivers/motor.h"

&#x20; |

&#x20; +-- #include "../drivers/servo.h"

&#x20; |

&#x20; +-- #include "../drivers/ultrasonic.h"

&#x20; |

&#x20; +-- #include "../drivers/color\_sensor.h"

&#x20; |

&#x20; v

Hardware driver implementations

&#x20; |

&#x20; v

PIC18F4520 peripherals and external hardware

```



Including a header gives the compiler access to its declarations. It does not automatically compile the corresponding `.c` file.



Each source file must be included in the MPLAB X project so XC8 can compile and link it.



Do not use `#include` to include another `.c` file.



\## 6. Adding Application Files to MPLAB X



When the files are created:



1\. Open the `conveyor` project in MPLAB X.

2\. In the \*\*Projects\*\* tab, right-click \*\*Source Files\*\*.

3\. Select \*\*Add Existing Item\*\* and choose `app/sorting.c`.

4\. Under \*\*Header Files\*\*, add `app/sorting.h` for project navigation.

5\. Verify that the driver source files are also part of the project.

6\. Build the project using XC8 and resolve any compilation or linker errors.



The physical folder structure does not automatically determine which source files MPLAB compiles.



\## 7. Development Guidelines



\- Keep the application logic separate from hardware driver implementations.

\- Use the functions declared in driver headers instead of directly manipulating hardware registers from the application.

\- Maintain one clearly defined current state.

\- Use descriptive state and function names.

\- Avoid unnecessary blocking delays inside the main application loop.

\- Define clear conditions for entering and leaving each state.

\- Handle unknown colors, sensor failures, and other faults safely.

\- Do not start the conveyor during initialization until the system is ready.

\- Test each state and transition before running the complete conveyor.

\- Coordinate changes to driver interfaces with the teammates responsible for those modules.



\## 8. Suggested Development Sequence



1\. Create `sorting.h` with the state definitions and public functions.

2\. Create `sorting.c` with initialization and a basic state machine.

3\. Connect `sorting\_init()` and `sorting\_update()` to `main.c`.

4\. Verify the application builds with the required driver modules.

5\. Test block detection and conveyor stopping.

6\. Integrate color classification.

7\. Implement block positioning and sorting gate control.

8\. Implement reset and error handling.

9\. Test repeated sorting cycles.



\## Current Status



The application architecture has been proposed, but the sorting state machine has not yet been implemented.



The team will begin with a stop-and-sort system capable of identifying red and blue blocks, then expand functionality if time and hardware permit.

