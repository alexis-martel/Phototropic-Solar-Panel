# Phototropic Solar Panel

> [!NOTE]
> This was made as part of an integration project in the Winter 2025 term.

Code for an Arduino-powered solar panel assembly that follows the Sun around one rotational axis. The position of the Sun is determined using an array of photoresistors. For the sake of simplicity, there is no "Position 0" switch to home the stepper motor; it must be at the "zero" angle upon boot.

## Features

- Wi-Fi web interface to monitor realtime power output;
- SD card for logging bulk power output data.

## Parts used

- Espressif ESP32-S3 development board from Adafruit;
- Texas Instruments INA260 power monitor breakout board from Adafruit;
- Texas Instruments BQ24074 battery charger breakout board from Adafruit;
- Toshiba TB6612 motor driver breakout board from Adafruit;
- 200-step 12V 350mA NEMA-17 stepper motor;
- Cadmium-sulfide photoresistors.
