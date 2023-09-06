# Look ma, no CPU!!
## Demo-application for the Distributed Programmable Peripheral Interconnect (DPPI) in a Nordic nrf9160:


### The periperals involved are:

### Configuration

## Pre-Requisites:
- git installed
- vscode installed, with plugins "Remote Containers", "Cortex-Debug"
- docker installed

## How to build:
1. "git clone https://github.com/stemschmidt/lookMaNoCPU.git"
2. "cd lookMaNoCPU"
3. "git checkout pwm_led"
4. "code ." or open lookMaNoCPU in VSCode
5. in VSCode, select "reopen folder in container"
6. after docker image is downloaded and started, go to "TERMINAL" tab:
7. "west init -l application"
8. "west update"
9. "source zephyr/zephyr-env.sh"
10. "cd application"
11. "west build -b nrf9160dk_nrf9160_ns" and wait for the build to finish...

12. on the host, launch the JLinkGDBServer (e.g "JLinkGDBServerCL -if swd -device nRF9160_xxAA -vd")
13. on the host, launch the JLinkRTTViewer to see debug log (e.g "JLinkRTTViewer", select "Connection to J-Link" -> "Existing session")

14. Select "Run and Debug" icon in activity bar in VSCode --> Download binary and start debugging by launching "JLink" 
