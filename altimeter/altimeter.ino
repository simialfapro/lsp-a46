#include <Wire.h>
#include <MPU6050_tockn.h>
#include "I2Cdev.h"
#include <SD.h>

int ledping = 5;    // led pin
int ledpinr = 6;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");

  // Verify connection
  Serial.println("Testing device connections...");

  attachServos();  // Function to attach servo objects to pins
  testMotors();    // Testing Motors
  pinMode(ledping, OUTPUT);
  pinMode(ledpinr, OUTPUT);

  // check if sd card module is working
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) { 
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
  digitalWrite(ledpinr, HIGH);
}

void loop() {
  getAltitude();
}

void getAltitude() {
  // Read raw accel measurements from MPU6050
  int ax = mpu6050.getAccX();

  // convert raw accel data to G
  float accelX_G = ax / 2048.0;

  // initialize the time variables
  static unsigned long startTime = 0;
  static unsigned long currentTime = 0;
  static bool movingUp = false;

  if (accelX_G > 0.1) {
    // If the sensor is moving upwards, start time
    if (!movingUp) {
      startTime = millis();
      movingUp = true;
    }
    currentTime = millis();
  } else {
    // Reset time variables if the sensor is not moving upwards
    startTime = 0;
    currentTime = 0;
    movingUp = false;
  }
  float deltaTime = (currentTime - startTime) / 1000.0; // Convert milliseconds to seconds

  // Calculate X-height using the formula: 0.5 * acceleration along X-axis * time^2
  float xHeight = 0.5 * accelX_G * deltaTime * deltaTime;

  String comma = String(',');
  String accelXraw = String(ax);
  String timer = String(currentTime);

  String Baro_data = String(accelXraw + comma + xHeight + comma + timer);
  Serial.println(Baro_data);

  // Write the data to the SD card
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(Baro_data);
    dataFile.close();
  }
  delay(1);
}





