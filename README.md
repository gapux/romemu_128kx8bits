# romemu32p
### Abstruct
A ROM Emulator but writing data from target processor is allowed (imcomplete dual-port RAM style.)
This example uses fast SRAM. So, adding some kind of bus arbiter will make this pretty nice dual-port SRAM with several tens of nanoseconds access time.

### * Main Parts
    * Static RAM 
    * Paspberry Pi Pico (USB interface and host side data manipulation)
    * Tri-state bidirectional bus buffers (hostside must be a 5V-tolerant type) 
    * Tri-state D-Latches (hostside address line expanders and switches, TTL or TTL compatible input level required.)
    * Tri-state Unidirectional Bus Buffers (targetside address and control lines switches)
    * Some logic gates
    * Adequate length of reset signal output cord
