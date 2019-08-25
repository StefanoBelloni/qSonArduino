# qSonArduino v0.1 #
## An Arduino Sonar for the 28BYJ-48 stepper motor using ULN2003 driver board and Qt to Display ##

#### created by Stefano Belloni, 8/2019 ####

this is a small Sonar build with arduino and Qt

![Alt text](res/sonar_bb.png?raw=true "Arduino Schematics")
![Alt text](res/q_sonar.png?raw=true "Radar")
![Alt text](res/sonar_metal.jpg?raw=true "Radar")

## Build

Follow the (incomplete) fritzing schema. 
 - the motor is connected with a motor driver (X113647 Stepper Driver Board) to arduino
 - the motor is connected to and external Power supply 
 - mount on the stepper monitor the Ultrasonic Sensor 
 - adjust the parameters to your environment, for example steps pro revolutions speed ... 
 - use the potentiometer to incerase or decrease the speed of revolution.

Build and run the Qt radar.

## Usage 

Connect Arduino with the usb cable and run the Qt application: you should see on the monitor of your computer the tipical screen of a sonar and some red point (vanisching with the time) if there is something near the Ultrasonic sensor.
The Sonar opens the file descriptor "/dev/ttyACM0" and try to reconnect if more then 1000000 in a row are met: usually there are around maximum 3000 invalid read.
If these parameters do not fit, one should change them, for the moment, in the source code.

For the Ultrasonic sensor has a timeout of 10000 microseconds, and that means that the radar has a maximum vision of 170 cm. 
The maximum distance to display an object or the mapping from distance to desktop size can be find in the function qsb_sonar.cpp, QsbSonar::forceUpdate.
