#include <SoftwareSerial.h>

const byte PACKET_LENGTH = 69;
const byte START_BYTES[] = {0x03, 0x09};
const byte DATA_START_INDEX = 3;
const byte DATA_END_INDEX = 66;
const byte NUM_DATA_POINTS = DATA_END_INDEX - DATA_START_INDEX + 1;

SoftwareSerial SSerial(5,4); // rx, tx

void setup() 
{
  Serial.begin(115200); 
  SSerial.begin(9600);
  Serial.print("TEST");
}

bool keyword = false; 

void ReadPacket()
{
  byte packet[PACKET_LENGTH];  
  int nBytes = SSerial.readBytes(packet, PACKET_LENGTH);

  if (nBytes >= PACKET_LENGTH) 
  {
    // Variables to store calculations
    int maxTemp = -127; // Minimum possible 8-bit integer value
    int minTemp = 127;  // Maximum possible 8-bit integer value
    int maxIndex = -1; 
    int minIndex = -1; 
    int sumTemp = 0;

    // Find maximum, minimum, and sum of temperatures
    for (byte i = DATA_START_INDEX; i <= DATA_END_INDEX; i++) 
    {
      int temperature = packet[i];
      if (temperature > maxTemp) {
        maxTemp = temperature;
        maxIndex = i - DATA_START_INDEX; 
      }
      if (temperature < minTemp) {
        minTemp = temperature;
        minIndex = i - DATA_START_INDEX; 
      }
      sumTemp += temperature;
    }

    // Calculate average temperature
    float avgTemp = static_cast<float>(sumTemp) / NUM_DATA_POINTS;

    // Output results
    Serial.print("Max Temperature: ");
    Serial.print(maxTemp);
    Serial.print(" (Index: ");
    Serial.print(maxIndex);
    Serial.println(")");

    Serial.print("Min Temperature: ");
    Serial.print(minTemp);
    Serial.print(" (Index: ");
    Serial.print(minIndex);
    Serial.println(")");

    Serial.print("Average Temperature: ");
    Serial.println(avgTemp);      
  }
}

void loop() 
{
  if (SSerial.available() > 0) 
  {
    byte ch = SSerial.read();
    if (keyword == false && ch == START_BYTES[0])
    {
      keyword = true; 
    }
    else if(keyword == true && ch == START_BYTES[1])
    {
      ReadPacket(); 
      keyword = false; 
    }
    else
    {
      keyword = false; 
    }
  }
}
