# gy-mcu8833

ESPHome test code for GY-MCU8833 8x8 array infrared thermometer. 

Unlike AMG8833 has I2C port, MCU8833 uses serial communication. I can't find ESPHome code(.YML) for MCU8833, so I implemented a simple custom sensor. Currently, there is no person presence information and 64 temperatures published to Home Assistant. Only maximum and minimum temperature among 64 pixels are available.   

refer to https://imky.tistory.com/72 (in Korean)

AMG8833 ESPHome implementation can be found in https://github.com/TheRealWaldo/AMG8833-ESPHOME
