#include <MPU6050_tockn.h>
#include <Servo.h>

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

long timer = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  attachServos();  // Function to attach servo objects to pins

  pinMode(ledping, OUTPUT);
  pinMode(ledpinr, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  activateIndicators();  // Function to activate LED and buzzer
  readAndPrintAngles();  // Function to read and print angles
  stabilizeServos();    // Function to stabilize and set servo angles
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
