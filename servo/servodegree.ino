#include <Servo.h>

int servopin1 = 7;  // Servo pin
int servopin2 = 8;
int servopin3 = 9;
int servopin4 = 10;

Servo servo1;   // Servo object
Servo servo2;
Servo servo3;
Servo servo4;

void setup() {
  Serial.begin(9600);

  attachServos();  // Function to attach servo objects to pins
}

void loop() {
  adjustMotorsTo90Degrees();
  delay(5000);  // Add a delay for observation (adjust as needed)
}

void attachServos() {
  servo1.attach(servopin1);
  servo2.attach(servopin2);
  servo3.attach(servopin3);
  servo4.attach(servopin4);
}

void adjustMotorsTo90Degrees() {
  Serial.println("Adjusting all motors to 90 degrees");

  servo1.write(96);
  servo2.write(95);
  servo3.write(95);
  servo4.write(97);

  delay(1000);  // Add a delay for observation (adjust as needed)
}
