
An adriuno project designed to read and display live Engine RPM from a vehicle's OBD-II port. It asks the vehicle's Engine Control Unit (ECU) over the CAN bus network and outputs the RPM data to an LCD screen.

## Hardware
* **Microcontroller:** Arduino Uno 
* **CAN Interface:** Seeed Studio CAN-BUS Shield V2 (MCP2515 Transceiver)
* **Display:** 16x2 LCD Screen
* **Power:** 5V battery or USB Power Bank 

## Wiring 
### CAN-Bus Shield (used to get the data from the ECU)
   Plugs directly into the Arduino board. 
* **CAN_H** -> OBD-II Pin 6
* **CAN_L** -> OBD-II Pin 14
* **SPI CS Pin** -> Arduino Pin 9

### 16X2 LCD Screen
* **RS** -> Arduino Pin 7
* **EN** -> Arduino Pin 8
* **D4** -> Arduino Pin 5
* **D5** -> Arduino Pin 2
* **D6** -> Arduino Pin 4
* **D7** -> Arduino Pin 3

## Libraries
* `SPI.h` - For communication with the CAN shield.
* `mcp_can.h` - Library for the MCP2515 CAN transceiver.
* `LiquidCrystal.h` - Library for the LCD screen.

## How it works
Setup:
         Initializes the 16x2 LCD screen. 
         Attempts to connect with the CAN shield.
        If it successfully connects to the MCP2515 chip, it switches the shield into "Normal Mode" so it can actively transmit data, clears the screen, and prints the "Rpm:" label.

Loop: 
* Sends OBD-II requests (Service `01`, PID `0x0C`) to address `0x7DF` to fetch live engine RPM.
* Calls handleRPM function which gets the 16-bit response from the ECU (Address `0x7E8`) and converts those bits using the  formula: `((A * 256) + B) / 4` to display human readable rpms
