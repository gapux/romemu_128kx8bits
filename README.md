# romemu32p
### Abstruct
A ROM Emulator but writing data from target processor is allowed (imcomplete dual-port RAM style.)
This example uses fast SRAM. So, adding some kind of bus arbiter will make this pretty nice dual-port SRAM with several tens of nanoseconds access time.
### * Main Features
    * 16-bit multiplex bus for hostside (AD0-AD7, A8/A16-A15/A23, ALE)
    * Designed to accept a 16M*8 StaticRAM by adding bus buffer ICs (not tested.)
    * Paspberry Pi Pico AS USB interface and host side data manipulation
    * Accepts Intel Hex format data transfer, as well as hexadecimal (0-9,A-F) type-in. 
    * Example of using a 128k*8 32-pin SRAM
    * Writes from target side is available (inhibitable by adding a switch.)
   
### * Main Parts
    * Static RAM (128k*8, 12ns. Address expandable upto 16M*8 by adding bus buffer ICs but not tested.)
    * Paspberry Pi Pico (USB interface and host side data manipulation)
    * Tri-state bidirectional bus buffers (hostside must be a 5V-tolerant type) 
    * Tri-state D-Latches (hostside address line expanders and switches, TTL or TTL compatible input level required.)
    * Tri-state Unidirectional Bus Buffers (targetside address and control lines switches)
    * Some logic gates
    * Adequate length of reset signal output cord
