// 2025-02-06 Alexis Andrew Martel
// Phototropic Solar Panel

#include <Stepper.h>
#include <Adafruit_INA260.h>
#include "SdFat.h"

int reduceAngle(int angle) {
  // Reduces the input angle to a value between 0 and 360º
  return angle % 360;
}

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}
