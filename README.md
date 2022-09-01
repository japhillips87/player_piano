# player_piano

This project is a DIY piano player. It is still a work in progress.

To make this project run, you will need to install a few libraries. First, you will need to install the ESP32 in the Arduino IDE. You can do that by following [this guide](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/).

Then you will need to search for a few libraries to install in the IDE. The first is [Arduino-BLE-MIDI](https://github.com/lathoub/Arduino-BLE-MIDI). If you install
this through the Arduino IDE, you don't need to install the midi library. But if you don't install it through the IDE, you need to install it from [here](https://github.com/FortySevenEffects/arduino_midi_library). And finally, you need the [PCA9635](https://github.com/RobTillaart/PCA9635) library.
