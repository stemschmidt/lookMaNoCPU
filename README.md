# lookMaNoCPU
Demo-application for the Distributed Programmable Peripheral Interconnect (DPPI) in a Nordic nrf9160:

> The demo shows how peripherals in the nrf9160 can be connected with a publish/subscribe pattern in Hardware (DPPI). The CPU > > is only used to setup the periperals, the interaction between the peripheral happens without CPU interaction! No interrupts! > Only events and tasks (https://infocenter.nordicsemi.com/topic/ps_nrf9160/gpiote.html).

> The periperals involved are:
>- a GPIO as input (Button 1 on the nrf9160dk)
>- a SPI-Master with a pre-defined configuration (number of bytes to read)
>- a GPIO to indicate the end of the processing (LED4 on the nrf9160dk) 
>
>- The button-gpio is configured to detect a LowToHi transition.
>- The SPI-Master is configured to read 10 bytes into the receive_buffer when it receives an event
>- The led-gpio is configured to toggle the gpio when it receives an event
>- The configuration of the DPPI is
>>1) button-gpio event is published on channel 0
>>2) the SPI task is subscribed on channel 0
>>3) the SPI event (after performing the data transmission) is published on channel 1
>>4) the led-gpio task is subscribed on channel 1

Add the receive_buffer to your watch window. Check that after the memset the buffer contains only 0x55. Continue with debugger to let the CPU do the setup and enter the while loop.
Now press button 1 and notice that led4 lights up.
Have a look at the receive_buffer: It contains 0xff in the first 4 entries (read from the MISO pin) and 0x55 in the remaining 4 entries.

Pre-Requisites:
- git installed
- vscode installed, with plugins "Remote Containers", "Cortex-Debug"
- docker installed

How to build:
1. "git clone https://github.com/stemschmidt/lookMaNoCPU.git"
2. "cd lookMaNoCPU"
3. "code ." or open lookMaNoCPU in VSCode
4. in VSCode, select "reopen folder in container"
5. after docker image is downloaded and started, go to "TERMINAL" tab:
6. "west init -l application"
7. "west update"
8. "source zephyr/zephyr-env.sh"
9. "cd application"
10. "west build -b nrf9160dk_nrf9160_ns" and wait for the build to finish...

11. on the host, launch the JLinkGDBServer (e.g "JLinkGDBServerCL -if swd -device nRF9160_xxAA -vd")
12. on the host, launch the JLinkRTTViewer to see debug log (e.g "JLinkRTTViewer", select "Connection to J-Link" -> "Existing session")

13. Select "Run and Debug" icon in activity bar in VSCode --> Download binary and start debugging by launching "JLink" 

