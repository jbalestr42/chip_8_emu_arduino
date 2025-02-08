# Chip 8 emulator for arduino

A simple chip 8 emulator written in C++ for arduino.
The goal was to turn a NES controller into a handheld controller to play chip 8 roms.
Even if there is not 16 keys like the chip 8, all keys are not always used.
To have a bit of control over the keys, there is a system to map a controller key to a chip 8 key for each rom.

It was a bit tricky to fit everything inside the controller but here is the final result.
![](/images/device.jpg)
![](/images/device.gif)

## Hardware
- Arduino Nano ESP32
- OLED screen 128x64px 0,96 inches -> Chip 8 resolution is 64x32 so it fits perfectly

## Schematic
![](/images/schematic.png)

### Improvement idea
It could be great to have a battery so we don't need a wire to play but the space is quite limited.
