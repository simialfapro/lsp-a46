#include <MPU6050_tockn.h>
#include <Servo.h>

int ledping = 5;    // led pin
int ledpinr = 6;

int servopin1 = 7;  // Servo pin
int servopin2 = 8;
int servopin3 = 9;
int servopin4 = 10;

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
  testMotors();
  pinMode(ledping, OUTPUT);
  pinMode(ledpinr, OUTPUT);
}

void loop() {
   activateIndicators();  // Function to activate LED and buzzer
   readAndPrintAngles();  // Function to read and print angles
   stabilizeServos(); // Stabilizes Servos
}

void attachServos() { // attaches all servopins
  servo1.attach(servopin1); 
  servo2.attach(servopin2);
  servo3.attach(servopin3);
  servo4.attach(servopin4);
}

void activateIndicators() { // activates LED indicators
  digitalWrite(ledpinr, HIGH);
  digitalWrite(ledping, HIGH);
}

void readAndPrintAngles() { // reads and prints angles
  mpu6050.update();
  if (millis() - timer > 1000) {
    Serial.print("angleX : ");
    Serial.print(mpu6050.getAngleX());
    Serial.print("  angleY : ");
    Serial.println(mpu6050.getAngleY());
    timer = millis(); 
    String Angle_Data = String(mpu6050.getAngleY() + "," + mpu6050.getAngleX() + "," + timer); 
    File dataFile = SD.open("angledata.txt", FILE_WRITE); // writes angleadata to sd-card
    if (dataFile) {
      dataFile.println(Angle_Data);
      dataFile.close();
    }
  }
}

void stabilizeServos() { // stabilizes all servos
  stabilizeServo2();
  stabilizeServo1();
  stabilizeServo3();
  stabilizeServo4();
}

void stabilizeServo2() { // servos stabilize functions
  if (mpu6050.getAngleX() > -1 && mpu6050.getAngleX() < 1) {
    servo2.write(90);
  } else {
    servo2.write(90 + mpu6050.getAngleY()); // reacts to angle from sensor
  }
}

void stabilizeServo1() {
  if (mpu6050.getAngleX() == 0) {
    servo1.write(90);
  } else {
    servo1.write(90 - mpu6050.getAngleY());
  }
}

void stabilizeServo3() {
  if (mpu6050.getAngleY() == 0) {
    servo3.write(90);
  } else {
    servo3.write(90 - mpu6050.getAngleX());
  }
}

void stabilizeServo4() {
  if (mpu6050.getAngleY() == 0) {
    servo4.write(90);
  } else {
    servo4.write(90 + mpu6050.getAngleX());
  }
}

void testMotors() { // Tests all motors
  int delayTime = 1000;  // Time to test each motor in milliseconds

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
}
