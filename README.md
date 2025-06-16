# romemu32p
A ROM Emulator but writing data from target processor is allowed (imcomplete dual-port RAM style.)
This example uses fast SRAM. So, adding some kind of bus arbiter will make this pretty nice dual-port SRAM with several tens of nanoseconds access time.

Ingredients
Paspberry Pi Pico for USB interface and host side data manipulation.
D-Latches for address line expantion.
Bus Buffers act as address and control lines multiplexers)
Some logic gates
Reset signal injector
