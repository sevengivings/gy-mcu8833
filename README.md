# gy-mcu8833

ESPHome test code for GY-MCU8833 8x8 array infrared thermometer. 

Unlike AMG8833 has I2C port, MCU8833 uses serial communication. I couldn't find ESPHome code(.YML) for MCU8833, so I implemented a simple custom sensor. Currently, only maximum and minimum temperature among 64 pixels are available to Home Assistant.   

Sample dump in hex using Realterm for Windows, I assumed that 0xA4 and 0x03 are keyword and I don't have any information about 09 42 0C B8(changed every time). 

About temperature value of Body, 0x20 means 32℃. 

Header 6 bytes, Body 64 bytes and Tail 1 byte(checksum 8bits modulo 256). 
```
A4 03 09 42 0C B8 
1A 1B 1A 1A 1A 1A 1A 1B 1A 1A 1A 1A 1A 1A 1A 1A 
1A 1A 1A 1A 1B 1A 1A 1A 1B 1A 1B 1B 1D 1E 1B 1B 
1B 1B 1B 1B 1C 1D 1E 1D 1B 1A 1B 1B 1B 1D 1E 1D 
1B 1B 1B 1B 1C 1E 1E 1D 1A 1B 1B 1B 1B 1E 1E 1D 
83
```

From https://www.scadacore.com/tools/programming-calculators/online-checksum-calculator/ I found that 0x83 is the result of CheckSum8 Modulo 256 of 0xA4 ~ 0x1D(70 bytes). So, added checksum verification.

I'm using this sensor to determine whether or not to activate the range hood.

![090609](https://github.com/sevengivings/gy-mcu8833/assets/2328500/ddd8145e-e9c4-475b-96eb-28d8879a7878)

The power of range hood is controlled by Home Assistant via IoT power plug. 

Generally it cannot be used as cooking thermometer because of inaccuracy and measuring limit. But, it can detect overheating of an empty skillet(ex. over 200℃).

For more information, refer to https://imky.tistory.com/72 and https://imky.tistory.com/60 (in Korean)

Excellent AMG8833 ESPHome implementation can be found in https://github.com/TheRealWaldo/AMG8833-ESPHOME 
