/*
  2025-02-06 Alexis Andrew Martel
  Phototropic Solar Panel
*/
/*
  WARNING!
  --------
  All angle measures are in radians!
  Panel must be powered on at position zero to prevent cable management vortex hell!
*/

#include <Stepper.h>          // Needed to control the motor through the TB6612 driver
#include <Adafruit_INA260.h>  // Needed to get voltage, amperage and power measurements from the INA260 breakout via I^2C
#include "SdFat.h"            // Needed for historical measurements of INA260 data

// Global constants
const int AXLE_MOTOR_STEPS = 100;
const double AXLE_GEAR_RATIO = 3 / 5;

// Global variables
double axle_angle = 0;

// Helper functions
int reduceAngle(double angle) {
  // Reduces the input angle to a value between 0 and 2π
  return fmod(angle, TWO_PI);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("A0: ");
  Serial.println(analogRead(A0));
  Serial.print("A1: ");
  Serial.println(analogRead(A1));
  Serial.print("A2: ");
  Serial.println(analogRead(A2));
  Serial.print("A3: ");
  Serial.println(analogRead(A3));
}
