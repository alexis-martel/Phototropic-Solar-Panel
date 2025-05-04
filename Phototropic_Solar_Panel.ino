/*
  2025-02-06 Alexis Andrew Martel
  Phototropic Solar Panel
*/
/*
  WARNING!
  --------
  All angle measures are in radians!
  Panel must be powered on at position zero to prevent cable management vortex hell!
  The `axle_angle` grows counterclockwise
*/

#include <Stepper.h>            // Needed to control the motor through the TB6612 driver
#include <Adafruit_INA260.h>    // Needed to get voltage, amperage and power measurements from the INA260 breakout via I^2C
#include "SdFat.h"              // Needed for historical measurements of INA260 data
#include <SPI.h>                // Needed for historical measurements of INA260 data
#include <Adafruit_NeoPixel.h>  // Needed to control the on-board NeoPixel
SdFat SD;

// Preprocessor constants
#define AXLE_MOTOR_STEPS 100
#define AXLE_MOTOR_STEP_SIZE 1
#define AXLE_MOTOR_SPEED 60
#define NUM_NEOPIX 1
#define PIN_PHOTORES_1 A1
#define PIN_PHOTORES_2 A2
#define SD_FAT_TYPE 3
#define SD_CS_PIN SS
#define AVG_INTERVAL 15  // Average wattage calculation interval in seconds
#define CSV_FILE "/data.csv"

// Global variables
double axle_angle = 0;
int left_photores = 0;
int right_photores = 0;
double wattage_sum = 0;
int wattage_count = 0;
unsigned long last_avg_time = 0;
Adafruit_INA260 ina260;
Stepper axle_motor(AXLE_MOTOR_STEPS, 4, 5, 6, 7);
File csvFile;

// Helper functions
double reduceAngle(double angle) {
  // Reduces the input angle to a value between 0 and 2π
  if (angle >= TWO_PI) {
    while (angle >= TWO_PI) {
      angle -= TWO_PI;
    }
  } else if (angle < 0) {
    while (angle < 0) {
      angle += TWO_PI;
    }
  }
  return angle;
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

void logToCSV(float voltage, float current, float power) {
  csvFile = SD.open(CSV_FILE, FILE_WRITE);
  if (csvFile) {
    csvFile.print(millis() / 1000);  // Timestamp in seconds
    csvFile.print(",");
    csvFile.print(voltage);
    csvFile.print(",");
    csvFile.print(current);
    csvFile.print(",");
    csvFile.println(power);
    csvFile.close();
  } else {
    Serial.println("Failed to open CSV file for writing.");
  }
}

void printCSVFile() {
  csvFile = SD.open(CSV_FILE, FILE_READ);
  if (csvFile) {
    while (csvFile.available()) {
      Serial.write(csvFile.read());
    }
    csvFile.close();
  } else {
    Serial.println("Failed to open CSV file for reading.");
  }
}

void setup() {
  // put your setup code here, to run once:
  axle_motor.setSpeed(AXLE_MOTOR_SPEED);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  // LED startup sequence
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize INA260
  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }

  // Initialize SD card
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Failed to initialize SD card");
    while (1);
  }

  // Create CSV file if it doesn't exist
  if (!SD.exists(CSV_FILE)) {
    csvFile = SD.open(CSV_FILE, FILE_WRITE);
    if (csvFile) {
      csvFile.println("Timestamp (s),Voltage (V),Current (A),Power (W)");
      csvFile.close();
    }
  }

  // TODO: Remove this statement for headless use
  while (!Serial.available()) {
    yield();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  axle_angle = reduceAngle(axle_angle);
  left_photores = analogRead(PIN_PHOTORES_1);
  right_photores = analogRead(PIN_PHOTORES_2);

  float voltage = ina260.readBusVoltage();
  float current = ina260.readCurrent();
  float power = ina260.readPower();

  // Log to CSV
  logToCSV(voltage, current, power);

  // Update wattage sum and count for averaging
  wattage_sum += power;
  wattage_count++;

  // Calculate average wattage over interval
  if (millis() - last_avg_time >= AVG_INTERVAL * 1000) {
    double avg_wattage = wattage_sum / wattage_count;
    Serial.print("Average Wattage: ");
    Serial.println(avg_wattage);
    wattage_sum = 0;
    wattage_count = 0;
    last_avg_time = millis();
  }

  Serial.print("Left: ");
  Serial.print(left_photores);
  Serial.print(", Right: ");
  Serial.print(right_photores);
  Serial.print(", Angle: ");
  Serial.println(axle_angle);

  // Check which photoresistor is higher
  if (left_photores > right_photores) {
    Serial.println("Moving counterclockwise");
    axle_motor.step(-AXLE_MOTOR_STEP_SIZE);
    axle_angle += (AXLE_MOTOR_STEP_SIZE / AXLE_MOTOR_STEPS) * TWO_PI;
  } else if (right_photores > left_photores) {
    Serial.println("Moving clockwise");
    axle_motor.step(AXLE_MOTOR_STEP_SIZE);
    axle_angle -= (AXLE_MOTOR_STEP_SIZE / AXLE_MOTOR_STEPS) * TWO_PI;
  } else {
    // Do nothing, the panel is perfectly centred
    Serial.println("Standing by");
  }

  // Print CSV file content on demand
  if (Serial.available()) {
    char input = Serial.read();
    if (input == 'p') {  // Press 'p' to print the CSV file
      printCSVFile();
    }
  }

  delay(300);
}
