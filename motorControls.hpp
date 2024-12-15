#pragma once

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



/*
void turnCounterclockwise(unsigned Delay = 10, unsigned leftSpeed = 255, unsigned rightSpeed = 127)
{
  driveMotorLEFT(BACKWARD, leftSpeed);
  driveMotorRIGHT(FORWARD, rightSpeed);
  delay(Delay);
}

void turnClockwise(unsigned leftSpeed = 127, unsigned rightSpeed = 255)
{
  driveMotorLEFT(FORWARD, leftSpeed);
  driveMotorRIGHT(BACKWARD, rightSpeed);
  delay(10);
}
*/
