#pragma once

void printEncoders()
{
   Serial.print(digitalRead(36));
   Serial.print(" ");
   Serial.print(digitalRead(39));
   Serial.print(" ");
   Serial.print(digitalRead(13));
   Serial.print(" ");
   Serial.print(digitalRead(14));
   Serial.println(" ");
}

void printSensorsValues()
{
  for(size_t i = 0; i<NUM_SENSORS; i++)
  {
    Serial.print(sensorsValues[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void printSensorsValuesAnalog()
{
  for(size_t i =0; i<NUM_SENSORS; i++)
  {
    Serial.print(analogRead(sensors[i]));
    Serial.print(" ");
  }
  Serial.println(" ");
}
