#pragma once

void readSensors()
{
  for(size_t i = 0; i<NUM_SENSORS; i++)
  {
    sensorsValues[i] = analogRead(sensors[i])>thresholdValues[i];
  }
}

//Calibration Area Start
void findThreshold()
{
  startMotors();
  //spin on axis clockwise
  driveMotorLEFT(FORWARD, 127);
  driveMotorRIGHT(BACKWARD, 255);

  int blackestValues[NUM_SENSORS] = {4096}, whitestValues[NUM_SENSORS] = {0};
  unsigned long startSpinning = millis();
  while(millis() - startSpinning < 3000)
  {
    for(size_t i =0; i<NUM_SENSORS; i++)
    {
      int val = analogRead(sensors[i]);
      if(val > whitestValues[i]) whitestValues[i] = val;
      if(val < blackestValues[i]) blackestValues[i] = val;
    } 
  }
  //average of each sensor's blackest and whitest is its threshold
  for(size_t i =0; i<NUM_SENSORS; i++)
  {
    thresholdValues[i] = (whitestValues[i] + blackestValues[i])/2;
  }
  stopMotors();
}

void saveThresholdValues() 
{
  for (size_t i = 0; i < NUM_SENSORS; i++) {
    EEPROM.put(i * sizeof(int), thresholdValues[i]);
  }
  Serial.println("Thresholds saved to EEPROM.");
}

void loadThresholdValues() 
{
  for (size_t i = 0; i < NUM_SENSORS; i++) 
  {
    EEPROM.get(i * sizeof(int), thresholdValues[i]);
  }
  Serial.println("Thresholds loaded from EEPROM.");
}

void printThresholdValues() 
{
  Serial.println("Threshold Values:");
  for (size_t i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(thresholdValues[i]);
  }
}

void calibrateThreshold()
{
   if (digitalRead(DRY_RUN) == HIGH) 
   {
    Serial.println("Button 16 pressed: Finding and saving thresholds...");
    findThreshold();
    saveThresholdValues();
  } 
  if (digitalRead(WET_RUN) == HIGH) 
  {
    Serial.println("Button 17 pressed: Retrieving and displaying thresholds...");
    loadThresholdValues();
    printThresholdValues();
  }
}
//Calibration Area End
