# lsp-a46
This repository contains code for a self-stabilizing Arduino model rocket module. The code is divided into three sections:

- Altimeter: This directory contains code specifically for height measurement. It is designed to capture the rocket's altitude.
- Servo: In this directory, you'll find code dedicated solely to the self-stabilization system. This portion of the code is responsible for controlling the servo motors, which serve to stabilize the rocket during flight.
- Main: This directory merges both codes together. It contains the main code that integrates both altitude measurement and the self-stabilization system

![Modelrocket-Module](/img/module.jpeg)

## Arduino Wiring Schematic:

![Modelrocket-Module](/img/schema.png)


> [!TIP]
> The Angles of Servos should be at 90 degree at default, sometimes fine adjustments are necessary.

    void stabilizeServo1() {
    if (mpu6050.getAngleX() == 0) {
        servo1.write(96);
    } else {
        servo1.write(96 + mpu6050.getAngleY());
    }
    }
    ...


## Dependencies:
Libraries:
- [MPU6050_tockn.h](https://github.com/tockn/MPU6050_tockn) 
- [I2Cdev.h](https://github.com/jrowberg/i2cdevlib)
