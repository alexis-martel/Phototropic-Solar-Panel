// 2025-02-06 Alexis Andrew Martel
// Phototropic Solar Panel

/*
  WARNING!
  All angle measures are in radians.
*/

// Libraries used
#include <Stepper.h>
#include <Adafruit_INA260.h>
#include "SdFat.h"

// Global variables
const int PHOTORESISTOR_PAIRS[4][2] = { { A0, A1 }, { A1, A2 }, { A2, A3 }, { A3, A0 } };

// Helper functions
int reduceAngle(int angle) {
  // Reduces the input angle to a value between 0 and 360º
  return angle % 360;
}

int getHighestPhotoresistorPair(int pairs[][2]) {
  // Returns the index of the pair with the highest combined detected luminosity
  int numPairs = sizeof(pairs) / sizeof(pairs[0]);
  for(;;) {

  }
}

float getBaseAngle(int pair, int totalPairs) {
  // Returns the base angle of the pair, to which will be added the linearly estimated middle-point from that angle
}

float getMiddlePointRemainder(int pair, int totalPairs) {
  // Returns a linear estimate of the remaining angular distance
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
