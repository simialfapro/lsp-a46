#include <MPU6050_tockn.h>
#include <Servo.h>

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

long timer = 0;

int currentMotor = 1;  // Variable to keep track of the current motor being tested

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  attachServos();  // Function to attach servo objects to pins
  testMotors();
  pinMode(ledping, OUTPUT);
  pinMode(ledpinr, OUTPUT);
}

void loop() {
   activateIndicators();  // Function to activate LED and buzzer
   readAndPrintAngles();  // Function to read and print angles
   stabilizeServos();
  // Test each motor individually
  // testMotors();
  // delay(100);  // Add a delay after each test to ensure sufficient time before moving to the next motor
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

void testMotors() {
  int delayTime = 1000;  // Time to test each motor in milliseconds

  switch (currentMotor) {
    case 1:
      Serial.println("Testing Motor 1");
      Serial.println("Moving Motor 1 to 0 degrees");
      servo1.write(0);  // Move Motor 1 to 0 degrees
      delay(delayTime);
      Serial.println("Moving Motor 1 to 180 degrees");
      servo1.write(180);  // Move Motor 1 to 180 degrees
      delay(delayTime);
      Serial.println("Moving Motor 1 to 90 degrees (center position)");
      servo1.write(90);  // Move Motor 1 to 90 degrees (center position)
      delay(delayTime);
      break;

    case 2:
      Serial.println("Testing Motor 2");
      Serial.println("Moving Motor 2 to 0 degrees");
      servo2.write(0);  // Move Motor 2 to 0 degrees
      delay(delayTime);
      Serial.println("Moving Motor 2 to 180 degrees");
      servo2.write(180);  // Move Motor 2 to 180 degrees
      delay(delayTime);
      Serial.println("Moving Motor 2 to 90 degrees (center position)");
      servo2.write(90);  // Move Motor 2 to 90 degrees (center position)
      delay(delayTime);
      break;

    case 3:
      Serial.println("Testing Motor 3");
      Serial.println("Moving Motor 3 to 0 degrees");
      servo3.write(0);  // Move Motor 3 to 0 degrees
      delay(delayTime);
      Serial.println("Moving Motor 3 to 180 degrees");
      servo3.write(180);  // Move Motor 3 to 180 degrees
      delay(delayTime);
      Serial.println("Moving Motor 3 to 90 degrees (center position)");
      servo3.write(90);  // Move Motor 3 to 90 degrees (center position)
      delay(delayTime);
      break;

    case 4:
      Serial.println("Testing Motor 4");
      Serial.println("Moving Motor 4 to 0 degrees");
      servo4.write(0);  // Move Motor 4 to 0 degrees
      delay(delayTime);
      Serial.println("Moving Motor 4 to 180 degrees");
      servo4.write(180);  // Move Motor 4 to 180 degrees
      delay(delayTime);
      Serial.println("Moving Motor 4 to 90 degrees (center position)");
      servo4.write(90);  // Move Motor 4 to 90 degrees (center position)
      delay(delayTime);
      break;
  }

  currentMotor++;  // Move to the next motor

  if (currentMotor > 4) {
    currentMotor = 1;  // Reset to the first motor after testing all motors
  }
}
