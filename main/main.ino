#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <SD.h>
#include <MPU6050_tockn.h>
#include <Servo.h>

MPU6050 accelgyro;
#define LED_PIN 13

int ledping = 7;    // led pin
int ledpinr = 6;

int servopin1 = 9;  // Servo pin
int servopin2 = 10;
int servopin3 = 11;
int servopin4 = 12;

Servo servo1;   // Servo object
Servo servo2;
Servo servo3;
Servo servo4;

MPU6050 mpu6050(Wire);

long timer = 0;

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

  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  attachServos();  // Function to attach servo objects to pins

  pinMode(ledping, OUTPUT);
  pinMode(ledpinr, OUTPUT);
}

void loop() {
  activateIndicators();  // Function to activate LED and buzzer
  readAndPrintAngles();  // Function to read and print angles
  getaltitude();
  delay(100);
  // stabilizeServos();    // Function to stabilize and set servo angles
}

void getaltitude() {
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
void attachServos() {
  servo1.attach(servopin1);
  servo2.attach(servopin2);
  servo3.attach(servopin3);
  servo4.attach(servopin4);
}

void activateIndicators() {
  digitalWrite(ledpinr, HIGH);
  digitalWrite(ledping, HIGH);
}

void readAndPrintAngles() {
  mpu6050.update();
    Serial.print("angleX : ");
    Serial.print(mpu6050.getAngleX());
    Serial.print("  angleY : ");
    Serial.println(mpu6050.getAngleY());
}

void stabilizeServos() {
  stabilizeServo2();
  stabilizeServo1();
  stabilizeServo3();
  stabilizeServo4();
}

void stabilizeServo2() {
  if (mpu6050.getAngleX() > -1 && mpu6050.getAngleX() < 1) {
    servo2.write(90);
  } else {
    servo2.write(90 - mpu6050.getAngleY());
  }
}

void stabilizeServo1() {
  if (mpu6050.getAngleX() == 0) {
    servo1.write(90);
  } else {
    servo1.write(90 + mpu6050.getAngleY());
  }
}

void stabilizeServo3() {
  if (mpu6050.getAngleY() == 0) {
    servo3.write(90);
  } else {
    servo3.write(90 + mpu6050.getAngleX());
  }
}

void stabilizeServo4() {
  if (mpu6050.getAngleY() == 0) {
    servo4.write(90);
  } else {
    servo4.write(90 - mpu6050.getAngleX());
  }
