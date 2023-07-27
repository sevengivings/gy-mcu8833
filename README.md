# gy-mcu8833

ESPHome test code for GY-MCU8833 8x8 array infrared thermometer. 

![093854](https://github.com/sevengivings/gy-mcu8833/assets/2328500/9ec427c5-6d64-486d-a1ee-d3703bac3157)

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

Generally it cannot be used as cooking thermometer because of inaccuracy and measuring limit. But, it can detect overheating of an empty skillet(ex. over 200℃).

I'm using this sensor to determine whether or not to activate the range hood.

![090609](https://github.com/sevengivings/gy-mcu8833/assets/2328500/ddd8145e-e9c4-475b-96eb-28d8879a7878)

The power of range hood is controlled by Home Assistant via IoT power plug. 

![iotplug](https://github.com/sevengivings/gy-mcu8833/assets/2328500/23b176ad-b6bd-4232-8fca-9d2a5e104d5b)

To determine to activate a fan, I'm using Trend sensor of Home Assistant. 

Trend sensors can be added to HA's configuration.yaml by adding the following. The values below are intuitive estimates, so you can change them to suit your environment. In my case, I'm getting data from the AMG8833 once every 5 seconds, so I'm reading a value 36 times every 180 seconds. The value of 0.0416667 is calculated by 7.5 degrees/180 seconds, which means that if there is a change of 7.5 degrees in 3 minutes, I want to set the binary sensor value called induction_thermal_rising to 'on'. 

```
# Trend sensor
binary_sensor:
  - platform: trend
    sensors:
      induction_thermal_rising:
        entity_id: sensor.thermal_sensor_max
        sample_duration: 180
        max_samples: 36
        min_gradient: 0.0416667
        device_class: heat
```

Trigger

```
platform: numeric_state
entity_id: sensor.thermal_sensor_max
above: 55
for:
  hours: 0
  minutes: 0
  seconds: 30
```

Condition 

```
condition: state
entity_id: binary_sensor.induction_thermal_rising
state: "on"
```

To turn off a fan, I use more simple trigger.

```
platform: numeric_state
entity_id: sensor.thermal_sensor_max
below: 50
for:
  hours: 0
  minutes: 1
  seconds: 30
```

For more information, refer to https://imky.tistory.com/72 and https://imky.tistory.com/60 (in Korean)

Excellent AMG8833 ESPHome implementation can be found in https://github.com/TheRealWaldo/AMG8833-ESPHOME 
