# Look ma, no CPU!!
## Demo-application for the Distributed Programmable Peripheral Interconnect (DPPI) in a Nordic nrf9160:

The demo shows how peripherals in the nrf9160 can be connected with a publish/subscribe pattern in Hardware (DPPI). The CPU is only used to setup the periperals, the interaction between the peripheral happens without CPU interaction! No interrupts! Only events and tasks (https://infocenter.nordicsemi.com/topic/ps_nrf9160/dppi.html).

### The periperals involved are:
- a GPIO as input (Button 1 on the nrf9160dk)
- a SPI-Master with a pre-defined configuration (number of bytes to read)
- a GPIO as output to indicate the end of the transfer (LED4 on the nrf9160dk) 
- the DPPI to connect the other peripherals

### Configuration
The button-gpio is configured to detect a LowToHi transition to create an event
```
nrf_gpio_cfg_input(BUTTON_1, NRF_GPIO_PIN_PULLUP);
nrf_gpiote_event_configure(NRF_GPIOTE, INPUT_CONFIG_0, BUTTON_1, NRF_GPIOTE_POLARITY_LOTOHI);
nrf_gpiote_event_enable(NRF_GPIOTE, INPUT_CONFIG_0);
```
The led-gpio task is configured to toggle the gpio
```
nrf_gpiote_task_configure(NRF_GPIOTE, OUTPUT_CONFIG, LED_4, NRF_GPIOTE_POLARITY_TOGGLE, 0);
nrf_gpiote_task_enable(NRF_GPIOTE, OUTPUT_CONFIG);
```
The SPI-Master task is configured to read 4 bytes into the receive_buffer
```
nrf_spim_frequency_set(NRF_SPIM0, NRF_SPIM_FREQ_2M);
nrf_spim_rx_buffer_set(NRF_SPIM0, receive_buffer, 4); // setup buffer and data to be read
nrf_spim_enable(NRF_SPIM0);
```
The configuration of the DPPI is
1) button-gpio event is published on channel 0
2) the SPI task is subscribed on channel 0
3) the SPI event (after performing the data transmission) is published on channel 1
4) the led-gpio task is subscribed on channel 1
```
nrf_gpiote_publish_set(NRF_GPIOTE, NRF_GPIOTE_EVENT_IN_0, trigger_spi_channel);
nrf_spim_subscribe_set(NRF_SPIM0, NRF_SPIM_TASK_START, trigger_spi_channel);

nrf_spim_publish_set(NRF_SPIM0, NRF_SPIM_EVENT_END, trigger_led_channel);
nrf_gpiote_subscribe_set(NRF_GPIOTE, NRF_GPIOTE_TASK_OUT_2, trigger_led_channel);
```

The while(1) loop at the end of main prints the content of the receive_buffer every second. The buffer is initialized with 0x55. When button 1 is pressed on the nrf9160dk you will see LED4 light up and the log shows that the first four bytes are now 0xff (the value that is read by the SPI with no external device attached)

**The CPU was not involved in the transfer of the data!**

Instead of triggering the LED task you could configure an interrupt to be triggered at the end of the SPI transmission. This way the CPU would receive an interrupt when the data from an external device is already transmitted and ready for the CPU to be consumed.

## Pre-Requisites:
- git installed
- vscode installed, with plugins "Remote Containers", "Cortex-Debug"
- docker installed

## How to build:
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

