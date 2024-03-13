#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <SD.h>

MPU6050 accelgyro;
#define LED_PIN 13

int ledping = 5;    // led pin
int ledpinr = 6;

int servopin1 = 9;  // Servo pin
int servopin2 = 8;
int servopin3 = 7;
int servopin4 = 10;

Servo servo1;   // Servo object
Servo servo2;
Servo servo3;
Servo servo4;

MPU6050 mpu6050(Wire);

void setup() {
  Wire.begin();

  Serial.begin(9600);

  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");
  accelgyro.initialize();

  // Verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  pinMode(LED_PIN, OUTPUT);

  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) { // Use the chip select pin of your SD card module
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  getAltitude();
}

void getAltitude() {
  // Read raw accel measurements from MPU6050
  int16_t ax, ay, az;
  accelgyro.getAcceleration(&ax, &ay, &az);

  // Convert raw accel data to G
  float accelX_G = ax / 2048.0;

  // Calculate X-height (very simplified method)
  // Note: This is a basic integration and may not be accurate
  float xHeight = 0.5 * accelX_G * 0.001;

  // Convert sensor data to strings
  String comma = String(',');
  String accelXraw = String(ax);
  String timer = String(millis());

  // Create a big string containing above strings
  String Baro_data = String(accelXraw + comma + xHeight + comma + timer);

  // Print the data to Serial
  Serial.println(Baro_data);

  // Write the data to the SD card
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println(Baro_data);
    dataFile.close();
  }

  delay(500);
}
