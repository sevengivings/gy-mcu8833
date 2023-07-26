# gy-mcu8833

ESPHome test code for GY-MCU8833 8x8 array infrared thermometer. 

Unlike AMG8833 has I2C port, MCU8833 uses serial communication. I couldn't find ESPHome code(.YML) for MCU8833, so I implemented a simple custom sensor. Currently, only maximum and minimum temperature among 64 pixels are available to Home Assistant.   

Sample dump in hex using Realterm for Windows, 03 09 42 0C and A4 might not be changed. For example 0x20 means 32℃. Header 5 bytes, Body 64 bytes and Tail 2 bytes. If I get the description of detailed protocol, I'll implement it. 

```
03 09 42 0C 86 
1F 1F 20 20 1F 20 20 20 20 20 20 20 20 20 20 20 
1F 20 20 20 20 20 20 20 20 20 20 20 20 20 20 20 
20 20 20 20 20 20 20 20 20 21 20 20 20 21 21 20 
20 21 21 21 21 21 20 1F 20 20 20 20 1F 20 20 1E 
84 A4
```

I'm using this sensor to determine whether or not to activate the range hood.

![090609](https://github.com/sevengivings/gy-mcu8833/assets/2328500/ddd8145e-e9c4-475b-96eb-28d8879a7878)

Generally it cannot be used as cooking thermometer because of inaccuracy and measuring limit. But, it can detect overheating of an empty skillet(ex. over 200℃).

For more information, refer to https://imky.tistory.com/72 (in Korean)

Excellent AMG8833 ESPHome implementation can be found in https://github.com/TheRealWaldo/AMG8833-ESPHOME 
