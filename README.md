# drumApp
A Bluetooth Low Energy enabled practice pad for practicing percussion/drum rudiments/patterns.

## Getting Started

The current firmware is a simple arduino .ino file which can be flashed to the esp32 dev board of your choice via the arduino ide.

### Prerequisites

* A ESP32 Development Board
* Arduino IDE
* Hardware stuff (to be added soon to github)

Additionally two Arduino libraries are needed: (These can be downloaded easily through the arduino ide package manager)

* Arduino Esp32 Library by espressif on [github](https://github.com/espressif/arduino-esp32)
* Esp32 BLE Arduino by nkolban on [github](https://github.com/nkolban/ESP32_BLE_Arduino)

### Installing

* Install the needed libraries using the arduino library manager

### Building (this will include hardware soon)

* open the .ino file, click build.
* make sure esp32device is attached and configured correctly and click compile.
* ta da!

## Next Steps

This arduino version of the hardware is essentially a placeholder/prototype for next version.
The original project was based on TI's cc2650 however after graduating and loosing the access to that hardware I quickly replaced with a ESP32, building wih arduino, to catch up and save time.
The version is being replaced with a FreeRTOS based version build using the espressif tools and toolchain to run on the same ESP32 dev Board.
Eventually a custom pcb and esp32 board will be developed however this will not happen until the mobile application has a satisfactory feature set.

## Authors

* **Nicholas Rossetti** - *Initial work* - [nrossetti](https://github.com/nrossetti)

See also the list of [contributors](https://github.com/nrossetti/BLE_drumPad/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments
<!--
* Hat tip to anyone whose code was used
* Inspiration
* etc
--!>
