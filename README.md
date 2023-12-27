# Fencing-recording-apparatus

Simple fencing recording apparatus controlled wirelessly via smartphone that uses Raspberry Pi Pico W microcontroller.

## Hardware

- Raspberry Pi Pico W
- TM1637 4-digit 7-segment LED display
- dual 7-segment LED display x2
- SCT2024CSOG LED driver x2
- LED diodes
- speaker

## Default Pinout

### Time display

| Raspberry Pi Pico W | TM1637 |
| ------------------- | ------ |
| 5V                  | VCC    |
| GND                 | GND    |
| GPIO 5              | CLK    |
| GPIO 4              | DIO    |

### Left score display

| Raspberry Pi Pico W | SCT2024CSOG |
| ------------------- | ----------- |
| 5V                  | VDD         |
| GND                 | GND         |
| GND                 | OE          |
| GPIO 19             | SDI         |
| GPIO 18             | CLK         |
| GPIO 20             | LA          |

### Right score display

| Raspberry Pi Pico W | SCT2024CSOG |
| ------------------- | ----------- |
| 5V                  | VDD         |
| GND                 | GND         |
| GND                 | OE          |
| GPIO 19             | SDI         |
| GPIO 18             | CLK         |
| GPIO 21             | LA          |

### Other elements

| Raspberry Pi Pico W | Element          |
| ------------------- | ---------------- |
| GPIO 13             | left yellow LED  |
| GPIO 12             | left red LED     |
| GPIO 11             | right green LED  |
| GPIO 10             | right yellow LED |
| GPIO 15             | speaker          |

## Cloning

```
git clone --recurse-submodules https://github.com/IrekGos/Fencing-recording-apparatus.git
```

## Building

```
mkdir build
cd build
cmake ..
make -j4
```
