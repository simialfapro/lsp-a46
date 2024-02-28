#include <MPU6050_tockn.h>
#include <Servo.h>
#include <Wire.h>
#include <MS5611.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <SPI.h>
#include <SD.h>

int ledping = 7;    // led pin
int ledpinr = 6;
int buzzer = 5;     // buzzer pin

int servopin1 = 9;  // Servo pin
int servopin2 = 10;
int servopin3 = 11;
int servopin4 = 12;

Servo servo1;   // Servo object
Servo servo2;
Servo servo3;
Servo servo4;

MPU6050 mpu6050(Wire);
MS5611 ms5611;

long timer = 0;
const int chipSelect = 4;
#define LED_PIN 13

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  attachServos();  // Function to attach servo objects to pins

  pinMode(ledping, OUTPUT);
  pinMode(ledpinr, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Initialize MS5611 sensor
  Serial.println("Initialize MS5611 Sensor");
  while(!ms5611.begin()) {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }

  // Get reference pressure for relative altitude
  referencePressure = ms5611.readPressure();

  // Check settings
  checkSettings();

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  pinMode(LED_PIN, OUTPUT);

  // Initializing SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
}

void loop() {
  activateIndicators();  // Function to activate LED and buzzer
  readAndPrintAngles();  // Function to read and print angles
  stabilizeServos();    // Function to stabilize and set servo angles
  readBarometerData();  // Function to read and log barometer data
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
  digitalWrite(buzzer, HIGH);
}

void readAndPrintAngles() {
  mpu6050.update();

  if (millis() - timer > 1000) {
    Serial.print("angleX : ");
    Serial.print(mpu6050.getAngleX());
    Serial.print("  angleY : ");
    Serial.println(mpu6050.getAngleY());

    timer = millis();
  }
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
}

void readBarometerData() {
  // Read raw accel measurements from device
  accelgyro.getAcceleration(&ax, &ay, &az);

  // Read raw pressure and temp values
  uint32_t rawTemp = ms5611.readRawTemperature();
  uint32_t rawPressure = ms5611.readRawPressure();

  // Read true temperature & Pressure
  double realTemperature = ms5611.readTemperature();
  long realPressure = ms5611.readPressure();

  // Calculate altitude
  float absoluteAltitude = ms5611.getAltitude(realPressure);
  float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);

  // Change variables to strings
  String comma = String(',');
  String absalt = String(absoluteAltitude); // Absolute altitude in meters.
  String temp = String(realTemperature); // Real temp in degrees, may over read initially, will take time for sensor to stabilize.
  String pressure = String(realPressure); // Pressure in pascals
  String accelXraw = String(ax); // Raw accel in X. Divide by 2048 to get a "G" reading.
  String timestamp = String(millis()); // Puts a millis timestamp on each string.

  // Make a big string containing above strings
  String baroData = String(absalt + comma + temp + comma + pressure + comma + accelXraw + comma + timestamp);

  Serial.println(baroData);

  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  
  // If the file is available, write to it
  if (dataFile) {
    dataFile.println(baroData); // Put baroData on the SD card
    dataFile.close();
    
    if (millis() > 10000) // Change this number to change alarm delay (1s = 1000ms)
    {
      tone(8, 1000); // Change the second number to alter the tone of the piezo alarm
    } else {
      noTone(8);
    }
  }
}
