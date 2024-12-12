#include <stdint.h>

#define RIGHT_IN1 22
#define RIGHT_IN2 23 
#define LEFT_IN1 4
#define LEFT_IN2 5

#define PWM_LEFT 18 
#define PWM_RIGHT 19 
#define STBY 21

#define MAX_SPEED 170;
uint8_t speedLEFT = 127; 
uint8_t speedRIGHT = 127 ; 

#define DRY_RUN 16
#define ACTUAL_RUN 17

#define A_ENCO_C1 36
#define A_ENCO_C2 39
#define B_ENCO_C1 13
#define B_ENCO_C2 14

#define END_LED 2

enum MotorDirection 
{
  FORWARD,
  BACKWARD,
};

enum Junction
{
  STRAIGHT,
  RIGHT,
  LEFT,
  STRAIGHT_RIGHT,
  STRAIGHT_LEFT,
  T,
  CROSS,
  DEAD_END,
  END,
};

Junction activeJunction =  STRAIGHT;

#define NUM_SENSORS 5

float Kp=0.9,Ki=0.007,Kd=0.1;
float error=0, P=0, I=0, D=0, PID_value=0;
float previous_error=0;

unsigned sensors[NUM_SENSORS] = {27, 26, 33, 35, 34}; //25 is busted, took 32 down with it {27, 26, 25, 33, 32, 35, 34}
int sensorsValues[NUM_SENSORS] = {0};
int thresholdValues[NUM_SENSORS] = {1000, 1000, 1000, 1000, 1000};

void printSensorsAnalog()
{
  for(size_t i =0; i<NUM_SENSORS; i++)
  {
    Serial.print(analogRead(sensors[i]));
    Serial.print(" ");
  }
  Serial.println(" ");
}

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

void printButtons()
{
  Serial.print("DRY_RUN");
  Serial.print(digitalRead(DRY_RUN) == HIGH ? "Pressed" : "Released");
  Serial.print("ACTUAL_RUN");
  Serial.println(digitalRead(ACTUAL_RUN) == HIGH ? "Pressed" : "Released");
}

void ledBlink()
{
    digitalWrite(END_LED, HIGH);
    delay(1000);
    digitalWrite(END_LED, LOW);
    delay(1000);
}


void stopMotors()
{
  digitalWrite(STBY, LOW);
}

void startMotors()
{
  digitalWrite(STBY, HIGH);
}

void driveMotorRIGHT(MotorDirection direction, uint8_t speed)
{
  if (direction == FORWARD) 
  {
    digitalWrite(RIGHT_IN1, HIGH);
    digitalWrite(RIGHT_IN2, LOW);
  } 
  else if (direction == BACKWARD) 
  {
    digitalWrite(RIGHT_IN1, LOW);
    digitalWrite(RIGHT_IN2, HIGH);
  }
  analogWrite(PWM_RIGHT, speed);
}

void driveMotorLEFT(MotorDirection direction, uint8_t speed) 
{
  if (direction == FORWARD) 
  {
    digitalWrite(LEFT_IN1, HIGH);
    digitalWrite(LEFT_IN2, LOW);
  } 
  else if (direction == BACKWARD) 
  {
    digitalWrite(LEFT_IN1, LOW);
    digitalWrite(LEFT_IN2, HIGH);
  }
  analogWrite(PWM_LEFT, speed);
}

void motorTest()
{
  static MotorDirection currentDirectionRIGHT = FORWARD;
  static MotorDirection currentDirectionLEFT = FORWARD;
  static unsigned long lastChangeTime = 0;

  if (millis() - lastChangeTime > 2000) 
  {
    currentDirectionRIGHT = (currentDirectionRIGHT == FORWARD) ? BACKWARD : FORWARD;
    currentDirectionLEFT = (currentDirectionLEFT == FORWARD) ? BACKWARD : FORWARD;

    driveMotorRIGHT(currentDirectionRIGHT, speedLEFT);
    driveMotorLEFT(currentDirectionLEFT, speedRIGHT);

    lastChangeTime = millis();
  }
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

void readSensors()
{
  for(size_t i = 0; i<NUM_SENSORS; i++)
  {
    sensorsValues[i] = analogRead(sensors[i])>thresholdValues[i];
  }
}

void turnCounterclockwise(unsigned leftSpeed = 90, unsigned rightSpeed = 127, unsigned milliseconds = 500)
{
  driveMotorLEFT(FORWARD, leftSpeed);
  driveMotorRIGHT(FORWARD, rightSpeed);
  delay(milliseconds);
}

void turnClockwise(unsigned leftSpeed = 127, unsigned rightSpeed = 90, unsigned milliseconds = 500)
{
  driveMotorLEFT(FORWARD, leftSpeed);
  driveMotorRIGHT(FORWARD, rightSpeed);
  delay(milliseconds);
}

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

void identifyJunction()
{
  if ((sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==1)&&(sensorsValues[3]==0)&&(sensorsValues[4]==0))
  {
    activeJunction = STRAIGHT;
  }
  else if ((sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==0)&&(sensorsValues[3]==1)&&(sensorsValues[4]==1))
  {
    activeJunction = RIGHT;
  }
  else if ((sensorsValues[0]==1)&&(sensorsValues[1]==1)&&(sensorsValues[2]==0)&&(sensorsValues[3]==0)&&(sensorsValues[4]==0))
  {
    activeJunction = LEFT;
  }
  else if ((sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==1)&&(sensorsValues[3]==1)&&(sensorsValues[4]==1))
  {
    activeJunction = STRAIGHT_RIGHT;
  }
  else if ((sensorsValues[0]==1)&&(sensorsValues[1]==1)&&(sensorsValues[2]==0)&&(sensorsValues[3]==0)&&(sensorsValues[4]==0))
  {
    activeJunction = STRAIGHT_LEFT;
  }
  else if ((sensorsValues[0]==1)&&(sensorsValues[1]==0)&&(sensorsValues[2]==0)&&(sensorsValues[3]==0)&&(sensorsValues[4]==1))
  {
    activeJunction = T;
  }
  else if ((sensorsValues[0]==1)&&(sensorsValues[1]==0)&&(sensorsValues[2]==1)&&(sensorsValues[3]==0)&&(sensorsValues[4]==1))
  {
    activeJunction = CROSS;
  }
  else if ((sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==0)&&(sensorsValues[3]==0)&&(sensorsValues[4]==0))
  {
    activeJunction = DEAD_END;
  }
  else if ((sensorsValues[0]==1)&&(sensorsValues[1]==1)&&(sensorsValues[2]==1)&&(sensorsValues[3]==1)&&(sensorsValues[4]==1))
  {
    activeJunction = END;
  }
}



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
  pinMode(ACTUAL_RUN, INPUT);
  
  //sensors
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(27, INPUT);

  //encoders
  pinMode(A_ENCO_C1, INPUT);
  pinMode(A_ENCO_C2, INPUT);
  pinMode(B_ENCO_C1, INPUT);
  pinMode(B_ENCO_C2, INPUT);

  //LED
  pinMode(END_LED, OUTPUT);
}


#include <EEPROM.h>

void saveThresholdValues(int* thresholdValues, size_t numSensors) {
  for (size_t i = 0; i < numSensors; i++) {
    EEPROM.put(i * sizeof(int), thresholdValues[i]);
  }
  Serial.println("Thresholds saved to EEPROM.");
}

void loadThresholdValues(int* thresholdValues, size_t numSensors) {
  for (size_t i = 0; i < numSensors; i++) {
    EEPROM.get(i * sizeof(int), thresholdValues[i]);
  }
  Serial.println("Thresholds loaded from EEPROM.");
}

void printThresholdValues(int* thresholdValues, size_t numSensors) {
  Serial.println("Threshold Values:");
  for (size_t i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(thresholdValues[i]);
  }
}

void loop()
{
   if (digitalRead(DRY_RUN) == HIGH) 
   {
    Serial.println("Button 16 pressed: Finding and saving thresholds...");
    findThreshold();
    saveThresholdValues(thresholdValues, NUM_SENSORS);
  } 
  if (digitalRead(ACTUAL_RUN) == HIGH) { // Button 17 pressed
    Serial.println("Button 17 pressed: Retrieving and displaying thresholds...");
    loadThresholdValues(thresholdValues, NUM_SENSORS);
    printThresholdValues(thresholdValues, NUM_SENSORS);
  }
}
