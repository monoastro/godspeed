#pragma once
float Kp=0.1,Ki=0,Kd=0;
float error=0, P=0, I=0, D=0, PID_value=0, previous_error=0;

float positionAnalog() 
{
  float weightedSum = 0;
  float sum = 0;

  for (int i = 0; i < NUM_SENSORS; i++) {
    // Read the sensor value once
    float sensorValue = analogRead(sensors[i]);
    float sensorWeight = (i - (NUM_SENSORS - 1) / 2.0) * 10;

    weightedSum += sensorValue * sensorWeight;
    sum += sensorValue;
  }
  if (sum < 1e-6) return 0;
  return weightedSum / sum;
}

float position()
{
  readSensors();
  if ((sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==1)&&(sensorsValues[3]==0)&&(sensorsValues[4]==0))
  {
    error = 0;
  }
  else if ((sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==0)&&(sensorsValues[3]==1)&&(sensorsValues[4]==0))
  {
    error = 10;
  }
  else if ((sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==0)&&(sensorsValues[3]==0)&&(sensorsValues[4]==1))
  {
    error = 20; 
  }
  else if ((sensorsValues[0]==0)&&(sensorsValues[1]==1)&&(sensorsValues[2]==0)&&(sensorsValues[3]==0)&&(sensorsValues[4]==0))
  {
    error = -10;
  }
  else if ((sensorsValues[0]==1)&&(sensorsValues[1]==0)&&(sensorsValues[2]==0)&&(sensorsValues[3]==0)&&(sensorsValues[4]==0))
  {
    error = -20;
  }
  return error;
}

#define I_MAX 100
#define MAX_PID 10
//Decision functions Start
void followLine() 
{
  error = 0 - position();
  
  P = error;
  I += error;
  D = error - previous_error;

  //I = constrain(I, -I_MAX, I_MAX);
  
  PID_value = Kp * P + Ki * I + Kd * D;

  //PID_value = constrain(PID_value, -MAX_PID, MAX_PID);
  previous_error = error;
  

  int leftSpeed = baseSpeed - PID_value;
  int rightSpeed = baseSpeed + PID_value;
  
  driveMotorLEFT(FORWARD, constrain(leftSpeed, 0, MAX_SPEED));
  driveMotorRIGHT(FORWARD, constrain(rightSpeed, 0, MAX_SPEED));
}

bool straight()
{
  return (sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==1)&&(sensorsValues[3]==0)&&(sensorsValues[4]==0);
}
void identifyJunction()
{
  if (straight())
  {
    activeJunction = STRAIGHT;
  }
  else if ((sensorsValues[0]==1))
  {
    activeJunction = LEFT;
  }
  else if ((sensorsValues[0]==1)&&(sensorsValues[1]==0)&&(sensorsValues[2]==1)&&(sensorsValues[3]==0)&&(sensorsValues[4]==1))
  {
    activeJunction = CROSS;
  }
  else if ((sensorsValues[0]==0)&&(sensorsValues[1]==1)&&(sensorsValues[2]==1)&&(sensorsValues[3]==1)&&(sensorsValues[4]==0))
  {
    activeJunction = T;
  }
  
  else if ((sensorsValues[0]==0)&&(sensorsValues[1]==0)&&(sensorsValues[2]==0)&&(sensorsValues[3]==0)&&(sensorsValues[4]==1))
  {
    activeJunction = RIGHT;
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

void driveBotLEFT()
{

  driveMotorRIGHT(FORWARD, 127);
  driveMotorLEFT(BACKWARD, 255);

  do
  {
    readSensors();
  }
  while(!straight());
}

void driveBotRIGHT()
{
  driveMotorLEFT(FORWARD, 127);
  driveMotorRIGHT(BACKWARD, 255);

  do
  {
    readSensors();
  }
  while(!straight());
}

void turn180()
{
  driveMotorRIGHT(FORWARD, 127);
  driveMotorLEFT(BACKWARD, 255);

  do
  {
    readSensors();
  }
  while(!straight());
} 
 
