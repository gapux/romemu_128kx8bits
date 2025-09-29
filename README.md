# romemu32pin
### Abstruct
A ROM Emulator (imcomplete dual-port RAM)
This example uses fast SRAM. Adding bus arbitrator will make this pretty nice dual-port SRAM with several tens of nanoseconds access time.
### * Main Features
    * 16-line multiplex bus for hostside RAM access (AD0-AD7, A8/A16-A15/A23.)
    * Expandable to accomodate a 16M*8 Static RAM with little modifications to hardware and software (not tested.)
    * Use of a Paspberry Pi Pico as USB interface and host side data manipulation
    * Accepts Intel Hex format data transfer, as well as direct type-in of hexadecimal (0-9,A-F) numbers. 
    * Example of using a 128k*8 32-pin SRAM
    * Writes from target side is available (inhibitable by adding a switch.)
   
### * Main Parts
    * Static RAM (This example uses HM678127 128k*8, 12ns t_AA.) 
    * Paspberry Pi Pico (USB interface and host side data manipulation)
    * Tri-state bidirectional bus buffers (hostside one must be a 5V-tolerant inout type) 
    * Tri-state D-Latches i.e. 74HCT573's (as hostside bus demultiplexer and bus switches, TTL or TTL compatible input level required.)
    * Tri-state Unidirectional Bus Buffers i.e. 74LS244's (targetside address and target/host control lines switches.)
    * Some logic gates (adjust timings.)
    * Adequate length of reset signal output cord.
