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

#include <Stepper.h>            // Needed to control the motor through the TB6612 driver
#include <Adafruit_INA260.h>    // Needed to get voltage, amperage and power measurements from the INA260 breakout via I^2C
#include "SdFat.h"              // Needed for historical measurements of INA260 data
#include <Adafruit_NeoPixel.h>  // Needed to control the on-board NeoPixel

// Preprocessor constants
#define AXLE_MOTOR_STEPS 100
#define AXLE_MOTOR_SPEED 60
#define NUM_NEOPIX 1
#define PIN_PHOTORES_1 A0
#define PIN_PHOTORES_2 A1

// Global constants
const double AXLE_GEAR_RATIO = 3 / 5;

// Global variables
double axle_angle = 0;

Adafruit_NeoPixel neopixel(NUM_NEOPIX, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
Stepper axle_motor(AXLE_MOTOR_STEPS, 4, 5, 6, 7);

// Helper functions
int reduceAngle(double angle) {
  // Reduces the input angle to a value between 0 and 2π
  return fmod(fmod(angle, TWO_PI) + TWO_PI, TWO_PI);
}

void rotateToAngle(double target_angle, Stepper motor) {
  // Rotates the axle to a specific absolute angle
  double rotate_by_angle = target_angle - axle_angle;
  rotateByAngle(rotate_by_angle, motor);
}

void rotateByAngle(double rotate_by_angle, Stepper motor) {
  // Rotates the axle by the specified angle
  double new_angle = rotate_by_angle + axle_angle;
  if (new_angle > TWO_PI || new_angle < 0) {
    rotateToAngle(reduceAngle(new_angle), motor);
  } else {
    motor.step((rotate_by_angle * AXLE_MOTOR_STEPS) / TWO_PI);
    axle_angle = reduceAngle(axle_angle + rotate_by_angle);
  }
}

void blinkColor(uint32_t color, int duration, Adafruit_NeoPixel pixels) {
  pixels.setPixelColor(0, color);
  pixels.show();
  delay(duration);
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  delay(250);
}

void setup() {
  // put your setup code here, to run once:
  neopixel.begin();
  // Long green blink
  blinkColor(neopixel.Color(0, 255, 0), 1000, neopixel);
  // Long yellow blink
  blinkColor(neopixel.Color(255, 255, 0), 1000, neopixel);
  // Long red blink
  blinkColor(neopixel.Color(255, 0, 0), 1000, neopixel);
  // Two short blue blinks
  blinkColor(neopixel.Color(0, 0, 255), 250, neopixel);
  delay(250);
  blinkColor(neopixel.Color(0, 0, 255), 250, neopixel);

  axle_motor.setSpeed(AXLE_MOTOR_SPEED);
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
}
