#include "esphome.h"

class MCU8833Component : public PollingComponent, public UARTDevice {
 public:
  MCU8833Component(UARTComponent *parent) : PollingComponent(5000), UARTDevice(parent) {}

  Sensor *max_temperature = new Sensor();
  Sensor *min_temperature = new Sensor();
  Sensor *avg_temperature = new Sensor();
  Sensor *min_index = new Sensor();
  Sensor *max_index = new Sensor();

  // GY-MCU8833 serial protocol unknown 
  // HEADER = 6 bytes, BODY = 64 bytes, TAIL = 1 byte(checksum of 0 ~ 69th bytes)  
  const byte PACKET_LENGTH = 71;  
  const byte NUM_CELL = 64; 
  const byte START_BYTES[2] = {0xA4, 0x03};
  const int MAX_BUFFER = 512; 

  void setup() override 
  {
    // nothing to do here
  }

  // called 60 times per second
  void loop() override
  {
    // nothing to do here 
  }

  // for PollingComponent 
void update() override
  {
    bool read_success = false; 
    byte packet[MAX_BUFFER];   
    byte read_index = 0; 

    while (available()) 
    {
      byte ch; 
      read_success = read_byte(&ch); 
      if (!read_success)
      {
        break; 
      }
      else
      {
        packet[read_index++] = ch; 
      }
      if (read_index > MAX_BUFFER - 1)
      { 
        break; 
      }
    }

    if(read_index >= PACKET_LENGTH)
    {
      // Variables to store calculations
      float sumTemp = 0;
      float maxTemp = -127; // Minimum possible 8-bit integer value
      float minTemp = 127;  // Maximum possible 8-bit integer value
      int maxIndex = -1; 
      int minIndex = -1; 
      byte packet_selected[PACKET_LENGTH]; 

        // Find keyword 
      int keyword = 0;
      byte data_start = 0; 
      byte data_index = 0; 
      byte checksum = 0; 

      for (byte i = 0; i < read_index; i++)
      {
        if (keyword == 0 && packet[i] == START_BYTES[0])
        {
          keyword = 1; 
          packet_selected[data_index++] = START_BYTES[0]; 
        }
        else if(keyword == 1 && packet[i] == START_BYTES[1]) 
        {
          keyword = 2; 
          packet_selected[data_index++] = START_BYTES[1]; 
        }
        else if(keyword == 2) 
        {
            if (data_index == (PACKET_LENGTH - 1))
            {
              checksum = packet[i];
              break; 
            }
            else
            {
              packet_selected[data_index++] = packet[i]; 
            }
        }
      }

      // ignore incomplete packet 
      if (data_index != (PACKET_LENGTH - 1)) return; 

      // verify packet : Checksum 8bits modulo 256 
      int checksum_calc = 0; 
      for (byte i = 0; i < PACKET_LENGTH - 1; i++)
      {
        checksum_calc += (int)packet_selected[i];
      }

      byte checksum_result = (byte)(checksum_calc % 256); 
      if (checksum_result != checksum)
      {
          return; 
      }

      // Find maximum, minimum, and sum of temperatures
      for (byte i = 6; i < PACKET_LENGTH - 1; i++) 
      {
        signed char temp = static_cast<signed char>(packet_selected[i]);
        float temperature = static_cast<float>(temp); 
        if (temperature > maxTemp) {
          maxTemp = temperature;
          maxIndex = i + 1; 
        }
        if (temperature < minTemp) {
          minTemp = temperature;
          minIndex = i + 1; 
        }
        sumTemp += temperature;
      }

      // Calculate average temperature
      float avgTemp = sumTemp / NUM_CELL;

      max_temperature->publish_state(maxTemp);
      min_temperature->publish_state(minTemp);
      min_index->publish_state(minIndex);
      max_index->publish_state(maxIndex);
      avg_temperature->publish_state(avgTemp);
    }
  }
};

