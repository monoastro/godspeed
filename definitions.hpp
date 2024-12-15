#pragma once
#include <Arduino.h>

#define RIGHT_IN1 22
#define RIGHT_IN2 23 
#define LEFT_IN1 4
#define LEFT_IN2 5

#define PWM_LEFT 18 
#define PWM_RIGHT 19 
#define STBY 21

#define MAX_SPEED 255

#define DRY_RUN 16
#define WET_RUN 17

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
  T,
  CROSS,
  DEAD_END,
  END,
  STRAIGHT_RIGHT,
  STRAIGHT_LEFT,
};
Junction activeJunction =  STRAIGHT;

#define NUM_SENSORS 5


#define baseSpeed 150

unsigned sensors[NUM_SENSORS] = {27, 26, 33, 35, 34}; //25 is busted, took 32 down with it {27, 26, 25, 33, 32, 35, 34}
int sensorsValues[NUM_SENSORS] = {0};
int thresholdValues[NUM_SENSORS] = {1200, 951, 1405, 1730, 1923};
