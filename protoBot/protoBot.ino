#include <Arduino.h>
#include <stdint.h>

const int PWM_B = 18; 
const int B_IN2 = 5;
const int B_IN1 = 4;

const int A_IN1 = 22; 
const int A_IN2 = 23; 
const int PWM_A = 19; 

const int STBY = 21;

uint8_t speedA = 255; 
uint8_t speedB = 255; 

#define BUTTON1_PIN 16
#define BUTTON2_PIN 17

enum MotorDirection 
{
  FORWARD,
  BACKWARD
};

void readSensors()
{
  Serial.print(analogRead(34));
  Serial.print(" ");
  Serial.print(analogRead(35));
  Serial.print(" ");
  Serial.print(analogRead(32));
  Serial.print(" ");
  Serial.print(analogRead(33));
  Serial.print(" ");
  Serial.print(analogRead(25));
  Serial.print(" ");
  Serial.print(analogRead(26));
  Serial.print(" ");
  Serial.print(analogRead(27));
  Serial.println(" ");
}

void readEncoders()
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


void setup() 
{
  Serial.begin(115200);
  
  //motor driver
  pinMode(A_IN1, OUTPUT);
  pinMode(A_IN2, OUTPUT);
  pinMode(B_IN1, OUTPUT);
  pinMode(B_IN2, OUTPUT);
  pinMode(PWM_A, OUTPUT);
  pinMode(PWM_B, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
  
  //sensors
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);
  pinMode(27, INPUT);

  //encoders
  pinMode(36, INPUT);
  pinMode(39, INPUT);

  pinMode(13, INPUT);
  pinMode(14, INPUT);

  //Input Pins
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(BUTTON2_PIN, INPUT);

  //LED
  pinMode(2, OUTPUT);
}

void buttonTest()
{
  int button1State = digitalRead(BUTTON1_PIN);
  int button2State = digitalRead(BUTTON2_PIN);
  Serial.print("Button 1: ");
  Serial.print(button1State == HIGH ? "Pressed" : "Released");
  Serial.print(" | Button 2: ");
  Serial.println(button2State == HIGH ? "Pressed" : "Released");
}

void ledBlink()
{
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);
}

void driveMotorA(MotorDirection direction, uint8_t speed)
{
  if (direction == FORWARD) 
  {
    digitalWrite(A_IN1, HIGH);
    digitalWrite(A_IN2, LOW);
  } 
  else if (direction == BACKWARD) 
  {
    digitalWrite(A_IN1, LOW);
    digitalWrite(A_IN2, HIGH);
  }
  analogWrite(PWM_A, speed);
}

void driveMotorB(MotorDirection direction, uint8_t speed) {
  if (direction == FORWARD) 
  {
    digitalWrite(B_IN1, HIGH);
    digitalWrite(B_IN2, LOW);
  } 
  else if (direction == BACKWARD) 
  {
    digitalWrite(B_IN1, LOW);
    digitalWrite(B_IN2, HIGH);
  }
  analogWrite(PWM_B, speed);
}

void motorTest()
{
  static MotorDirection currentDirectionA = FORWARD;
  static MotorDirection currentDirectionB = FORWARD;
  static unsigned long lastChangeTime = 0;

  if (millis() - lastChangeTime > 2000) 
  {
    currentDirectionA = (currentDirectionA == FORWARD) ? BACKWARD : FORWARD;
    currentDirectionB = (currentDirectionB == FORWARD) ? BACKWARD : FORWARD;

    driveMotorA(currentDirectionA, speedA);
    driveMotorB(currentDirectionB, speedB);

    lastChangeTime = millis();
  }
}

void loop() 
{
  motorTest();
}
