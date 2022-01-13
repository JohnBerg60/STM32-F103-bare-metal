# STM32-F103 minimun/blink based on cmsis

## Needed libraries
- ARM provides cmsis core
- ST provides device and link script and includes cmsis core, link below.
Copy both stucture as sub folders from this project. From the core arm folder, copy the startup assembly code for the device

## Configuration parameters
Edit the Makefile, to match the include and sources paths.

## Usefull links
- [STM32 Cube F1, including cmsis core, device and link script](https://github.com/STMicroelectronics/STM32CubeF1)