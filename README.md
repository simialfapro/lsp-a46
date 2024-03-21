# lsp-a46
This Repository includes the Code of a self-stabilizing Arduino-Modelrocket-Module.
The code is seperated in 3 Directorires:

- altimeter: contains Code only for Height measurement
- servo: contains Code only for self-stabilisation system. 
- main: merges both codes togheter. 

![Modelrocket-Module](/img/module.jpeg)

## Arduino Wiring Schematic 

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

