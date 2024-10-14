# Traffic Controller Project

## Overview
This project implements a traffic controller using a TIVA-C Launchpad, LEDs, a shift register, and a Finite State Machine (FSM) to manage an intersection and crosswalk. The system controls the timing of traffic lights (represented by LEDs) and the pedestrian signals, ensuring smooth and safe traffic flow.

## Project Components
- **Microcontroller**: TIVA-C Launchpad (TM4C123G)
- **LEDs**: Represent traffic lights and pedestrian signals
- **Switches**: Represents cars/ pedestrians at the intersection and is the input used to determine the next state of the system
- **8-Bit Shift Register**: Used to extend GPIO pins and control multiple LEDs
- **Breadboard**: For wiring the circuit
- **Finite State Machine (FSM)**: Implements the logic to transition between different traffic light states
- **SysTick Timer**: Used to manage delays for state transitions in the FSM

## Repository Structure
- `main.c`: Contains the main logic of the traffic controller, including the FSM implementation for managing the intersection and crosswalk.
- `PLL.c` and `PLL.h`: Implements the Phase-Locked Loop (PLL) to configure the system clock for precise timing.
- `SysTick.c` and `SysTick.h`: Implements the SysTick timer, which controls the delay for each state transition in the FSM.

## Features
- **Traffic Light Control**: LEDs simulate traffic lights for both the main intersection and the pedestrian crosswalk.
- **Crosswalk Management**: Separate states to handle pedestrian crossing requests.
- **State Transitions**: The system uses a FSM to control the flow of states, with precise timing managed by the SysTick timer.
- **Expandable**: The shift register allows control of more LEDs than the available GPIO pins on the TIVA-C Launchpad.

## How It Works
The program follows a defined FSM, with each state representing a different traffic light configuration. The next state is determined by the input from 3 switches, and take on binary values 0-7 (e.g. 100 means the crosswalk switch it pressed, 010, means cars are at the North side and 001 means cars are at the East side). Each represents the current configuration of each interstion's lights. (e.g. goN means the North light is green while the other the crosswalk and East lights are red), and the  The SysTick timer is used to introduce delays between state transitions, simulating real-world traffic light behavior. For example:
| state  | input:|   000   |   001   |   010   |   011   |   100   |   101   |   110   |   111   |
|--------|-------|---------|---------|---------|---------|---------|---------|---------|---------|
|  GoN   |       |   GoN   |   SlowN |   GoN   |   SlowN |   SlowN |   SlowN |   SlowN | SlowN   |
|SlowN   |       |   GoE   |   GoE   |   GoE   |   GoE   |goWalkN  |goWalkN  | goWalkN |   GoE   |
|  GoE   |       |   GoE   |   GoE   | SlowE   | SlowE   | SlowE   | SlowE   | SlowE   | SlowE   |
|SlowE   |       |   GoN   |   GoN   |   GoN   |   GoN   |goWalkE  |goWalkE  |goWalkE  |   GoN   |
|GoWalkN |       |   GoN   |   GoE   |   GoN   |   GoN   |   GoN   |   GoE   |   GoN   |   GoE   |
|SlowNW  |       |goWalkN  |goWalkN  |goWalkN  |goWalkN  |goWalkN  |goWalkN  |goWalkN  |goWalkN  |
|SlowEW  |       |goWalkE  |goWalkE  |goWalkE  |goWalkE  |goWalkE  |goWalkE  |goWalkE  |goWalkE  |
|GoWalkE |       |   GoE   |   GoE   |   GoN   |   GoE   |   GoE   |   GoE   |   GoN   |   GoE   |


## Setup and Installation
1. Connect the TIVA-C Launchpad to the breadboard with LEDs and the shift register as per the circuit design.
2. Clone this repository to your local machine.
3. Load the code onto the TIVA-C Launchpad using Code Composer Studio.
4. Run the program and observe the traffic light sequence controlled by the FSM.

## Resources

1. [SN74HC595 Shift Register Datasheet](https://www.ti.com/lit/ds/symlink/sn74hc595.pdf)
2. [Shift Register Theory](https://rheingoldheavy.com/shift-register-theory/)
3. [Shift Registers Tutorial by GreatScott (YouTube)](https://youtu.be/6fVbJbNPrEU?si=BPLyMBKmyskpaW18)
4. [Understanding Shift Registers (YouTube)](https://www.youtube.com/watch?v=kgABPjf9qLI)
5. [Blinking LED in Tiva C Board (Instructables)](https://www.instructables.com/Blinking-LED-in-Tiva-C-Board-TM4C123G/)
6. [4D Traffic Light FSM (Youtube)](https://youtu.be/kgABPjf9qLI?si=aExNJqR0bQXZ1kfy)
