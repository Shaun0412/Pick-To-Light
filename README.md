# PIC2Light – Pick-to-Light Client

A PIC16F877A-based Pick-to-Light client developed using Embedded C. The system receives stock-picking commands through UART, identifies packets intended for its configured node, displays the required quantity using 7-segment displays, and provides a visual pick indication.

## Overview

The Pick-to-Light client is designed as a node in a warehouse/shop-floor picking system. A server sends a stock-picking request containing the Node ID and required quantity. The client processes the request only when the received Node ID matches its configured Node ID.

The client allows the operator to modify the picked quantity using a keypad and sends the final quantity back to the server after confirmation.

## Features

- UART-based communication with a server
- Custom packet format for Node ID and stock quantity
- Configurable Node ID
- EEPROM-based persistent Node ID storage
- Node ID filtering for received packets
- 4-digit 7-segment display for stock quantity
- Keypad-controlled stock increment/decrement
- Timer0 interrupt-based 2 Hz pick indication
- Operation Mode and Configuration Mode
- Stock update through keypad
- Node ID configuration through keypad
- UART transmission of final picked quantity
- 8-bit stock quantity handling with boundary checks

## Hardware / Software

### Hardware

- PIC16F877A
- 4-digit 7-segment display
- 6-key keypad
- Pick indication LED
- Internal EEPROM
- UART interface

### Software

- Embedded C
- MPLAB X IDE
- XC8 Compiler
- PICSimLab

## System Flow

```text
              +----------------+
              |     Server     |
              +-------+--------+
                      |
                    UART
                      |
                      v
            +--------------------+
            |   PIC16F877A Node  |
            +--------------------+
                      |
          +-----------+-----------+
          |                       |
          v                       v
   7-Segment Display        Pick Indicator
          |
          v
       Keypad
          |
          v
   Modify Pick Quantity
          |
          +-------> UART -------> Server
```

## UART Packet Format

The client uses a simple delimited packet format:

```text
<Node_ID,Stock>
```

Example:

```text
<10,50>
```

Where:

- `10` → Node ID
- `50` → Required stock quantity

The client extracts both values and processes the packet only if the received Node ID matches its configured Node ID.

## Operating Modes

### Operation Mode

In Operation Mode, the client:

1. Waits for a UART packet.
2. Parses the received Node ID and stock quantity.
3. Checks whether the packet belongs to the current node.
4. Displays the requested stock quantity on the 7-segment display.
5. Activates the pick indicator at 2 Hz.
6. Allows the operator to increase or decrease the picked quantity.
7. Sends the final picked quantity to the server when ACK is pressed.
8. Stops the pick indication after transmission.

### Configuration Mode

Configuration Mode provides:

- Update Stock
- Set Node ID

The Node ID is stored in the PIC16F877A's internal EEPROM so that the configured value is retained after reset or power loss.

## Keypad Controls

| Key | Function |
|-----|----------|
| MODE | Toggle Operation / Configuration Mode |
| CONFIRM | Select configuration option |
| ACK | Confirm / transmit |
| INC | Increment stock / Node ID |
| DEC | Decrement stock / Node ID |

## Timer0 Pick Indication

Timer0 is configured to generate periodic interrupts for the pick indicator.

The system uses the Timer0 interrupt to toggle the indicator at the required rate, producing a **2 Hz blinking indication** during an active picking operation.

The blink state is disabled once the operator acknowledges the completed pick.

## EEPROM Node ID

The Node ID is stored in the PIC16F877A's internal EEPROM.

A marker byte is used to determine whether the EEPROM contains a valid configured Node ID. If the marker is not present, the client initializes the Node ID with the default value.

This prevents an erased EEPROM value (`0xFF`) from being interpreted as a valid Node ID.

## Interrupts

The project uses interrupts for time-critical operations:

- **UART Receive Interrupt**
  - Receives incoming packets.
  - Detects packet boundaries.
  - Stores received data in the receive buffer.
  - Signals the main application when a complete packet is available.

- **Timer0 Interrupt**
  - Maintains the pick indication timing.
  - Generates the periodic LED toggle required for the 2 Hz indication.

## Project Structure

```text
PIC2Light/
│
├── main.c
├── main.h
│
├── uart.c
├── uart.h
│
├── keypad.c
├── keypad.h
│
├── ssd.c
├── ssd.h
│
├── eeprom.c
├── eeprom.h
│
├── timer0.c
├── timer0.h
│
└── README.md
```

> The exact file structure may vary depending on the MPLAB X project configuration.

## Challenges and Debugging

Some of the important issues encountered during development included:

- Handling erased EEPROM values during Node ID initialization
- Persisting a changed Node ID correctly
- Handling UART receive overrun conditions
- Sharing UART receive data safely between the ISR and main application
- Parsing multi-digit Node IDs and stock quantities
- Calculating Timer0 timing for the required blinking frequency
- Selecting an appropriate counter width for the blink timing
- Handling 8-bit stock quantity boundaries
- Ensuring the pick indicator turns off after ACK
- Managing keypad input across different operating modes
- Controlling 7-segment display output during mode transitions

## Concepts Practiced

This project provided practical experience with:

- Embedded C
- PIC16F877A architecture
- GPIO
- UART communication
- Interrupt-driven programming
- Timer0
- EEPROM
- 7-segment displays
- Matrix/keypad input
- State-based application control
- Serial packet parsing
- Embedded debugging
- Resource-constrained programming

## Development Environment

```text
Microcontroller : PIC16F877A
Compiler        : XC8
IDE             : MPLAB X
Simulator       : PICSimLab
Language        : Embedded C
Communication   : UART
Baud Rate       : 9600
```

## Author

**Shaun Varghese**

Embedded C | PIC16F877A | Microcontrollers | Embedded Systems
