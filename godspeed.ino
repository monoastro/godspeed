#include <stdint.h>
#include <EEPROM.h>
#include "definitions.h"
#include "debug.h"
#include "motorControls.h"
#include "calibration.h"
#include "position.h"

void setup() 
{
  Serial.begin(115200);
  
  //motor driver
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);
  pinMode(PWM_RIGHT, OUTPUT);
  pinMode(PWM_LEFT, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  
  //Input Pins
  pinMode(DRY_RUN, INPUT);
  pinMode(WET_RUN, INPUT);
  
  //sensors
  for(size_t i = 0; i<NUM_SENSORS; i++)
  {
    pinMode(sensors[i], INPUT);
  }

  //encoders
  pinMode(A_ENCO_C1, INPUT);
  pinMode(A_ENCO_C2, INPUT);
  pinMode(B_ENCO_C1, INPUT);
  pinMode(B_ENCO_C2, INPUT);

  //LED
  pinMode(END_LED, OUTPUT);

  //debug calibration
  //while(1) calibrateThreshold();

  //auto-calibration
  //findThreshold();

}

void loop()
{
   start:
   if (digitalRead(DRY_RUN) == HIGH) 
   {
    Serial.println("Dry Run");
    delay(2000);
    startMotors();
    while(true)
    {
      //Serial.println(position());
      printSensorsValues();
      readSensors();
      identifyJunction();
      switch (activeJunction) 
      {
        case LEFT:
        case STRAIGHT_LEFT:
        case CROSS:
        case T:
            Serial.println("Driving left");
            driveBotLEFT();
            break;
            
        case STRAIGHT: 
        case STRAIGHT_RIGHT: 
            Serial.println("Driving straight");
            followLine();
            break;
        
        case RIGHT:
            Serial.println("Driving right");
            driveBotRIGHT();
            break;
        case DEAD_END:
            Serial.println("Turning Back");
            turn180();
            break;
        case END:
            stopMotors();
            goto start;
            break;
      }
    }
  } 
  if (digitalRead(WET_RUN) == HIGH) 
  {
    Serial.println("Wet Run");
    delay(2000);
    startMotors();
  }
}
