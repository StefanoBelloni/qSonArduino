/*
 * The following sketch is used to implement a
 * radar that identify a passing object and try
 * to follow it.
 * 
 * The distance sensor is mount on the stepper motor.
 * The stepper motor start to rotate, and when an object
 * is 'seen' it will try to follow it.
 * 
 * STEPS: connect the motor: https://www.arduino.cc/en/Tutorial/StepperSpeedControl:
 * the stepper is connected from pin 8 to pin 11.
 * 
 * 
 */

#include <Stepper.h>

// =========================================== //
// Stepper set up
// =========================================== //
const int stepsRevolution = 1054;
const int stepPerRevolution = 64;
const int stepperPin0 =  8;
const int stepperPin1 =  9;
const int stepperPin2 = 10;
const int stepperPin3 = 11;

const int trigPin = 5;
const int echoPin = 6;

const int btnPin = 13;

const int ledPin = 7;

const int timeout = 10000;

/* ============================================= */

Stepper stepper(
  stepPerRevolution, 
  stepperPin0,
  stepperPin1,
  stepperPin2,
  stepperPin3);

int motorSpeed = 200;
int stepsCount = 0;
double grads = 0.0;
int dir = 1;
double grads_pro_step = 180.0 / 1054.0;

long duration;
int distance;

/* =================================================================
 * SETUP FUNCTION
 * ================================================================= */
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  stepper.setSpeed(motorSpeed);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);
}


/* =================================================================
 * LOOP FUNCTION
 * ================================================================= */
void loop() {

  // put your main code here, to run repeatedly:
  
  if ( digitalRead( btnPin ) == HIGH ) {
     resetRevolution();
     return;
  }
  
  int steps = 5;

  stepper.step( dir * steps);
  stepsCount += steps;
  grads += steps * grads_pro_step * dir;
  
  // change direction if needed:
  if ( stepsCount > stepsRevolution ) {
    changeDirectionRevolution();
    Serial.println("Change");
  }

  // If too close, light the LED up.
  if ( ( distance = calculateDistance() ) < 40 ) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  
  Serial.print(distance);
  Serial.print(",");
  Serial.println(grads);
}

// upon reset, make more evident with leds and so on.
void resetRevolution() {
  changeDirectionRevolution();
  grads = 0.0;
  Serial.println("R");
  for ( int j = 0; j < 5; ++j ) {
    digitalWrite(ledPin, HIGH);
    delay(300);
    digitalWrite(ledPin, LOW);
    delay(300);
  }
}

// Function for calculating the distance measured by the Ultrasonic sensor

int calculateDistance(){ 
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, timeout); // Reads the echoPin, returns the sound wave travel time in microseconds

  if ( duration != 0 ) {    
    distance=  duration * 0.034 / 2;
  } else {                  
    distance = timeout  * 0.034 / 2;
  }
  // Serial.print(duration); Serial.print("; ");
  // Serial.println(distance);
  delayMicroseconds(timeout - duration);
  return distance;
}

int changeDirectionRevolution() {
  dir = -dir;
  stepsCount = 0;
}
