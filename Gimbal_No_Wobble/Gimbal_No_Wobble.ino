#include <Stepper.h>

// pins
#define xPin A0
#define yPin A1

// constants
float steps = 2048;
float degreesPerStep = 360 / steps;

// parameters
float maxAngle = 2;      // Sets max angle which gimbal can go
float factor = 1.15;      // Sets at which angle switches between slow and fast
int speed = 700;          // Speed of stepper

// variables
float x;
float y;
float xAngle;
float yAngle;
float xOffset;
float yOffset;
float xCurAngle;
float yCurAngle;

// Allows for mapping analog input to degree measure
float mapFloat(float x, float inMin, float inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

// Steppers
Stepper XStepper(steps / 64, 13, 11, 12, 10);
Stepper YStepper(steps / 64, 7, 5, 6, 4);

void setup() {
  Serial.begin(9600);

  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);

  // Calibration
  Serial.println();
  Serial.print("Calibrating... Please keep the stick at a neutral position then press enter... ");
  while(Serial.available() == 0) {}

  x = analogRead(xPin);
  y = analogRead(yPin);

  xOffset = mapFloat(x, 0, 1023, -1 * maxAngle, maxAngle);
  yOffset = mapFloat(y, 0, 1023, -1 * maxAngle, maxAngle);

  Serial.println("Done!");

  XStepper.setSpeed(speed);
  YStepper.setSpeed(speed);
}

void loop() {
  // Reads from joystick
  x = analogRead(xPin);
  y = analogRead(yPin);

  // Calculates angle based on analog input
  // Accounts for offset
  xAngle = mapFloat(x, 0, 1023, -1 * maxAngle + xOffset, maxAngle - xOffset) - xOffset;
  yAngle = mapFloat(y, 0, 1023, -1 * maxAngle + yOffset, maxAngle - yOffset) - yOffset;

  // Steps for x stepper
  if (abs(abs(xAngle) - abs(xCurAngle)) < degreesPerStep) {

  } else if (xCurAngle <= xAngle) {
    XStepper.step(1);
    xCurAngle = xCurAngle + (degreesPerStep);
  } else if (xCurAngle >= xAngle) {
    XStepper.step(-1);
    xCurAngle = xCurAngle - (degreesPerStep);
  }

  // Steps for y stepper
  if (abs(abs(yAngle) - abs(yCurAngle)) < degreesPerStep) {

  } else if (yCurAngle <= yAngle) {
    YStepper.step(1);
    yCurAngle = yCurAngle + (degreesPerStep);
  } else if (yCurAngle >= yAngle) {
    YStepper.step(-1);
    yCurAngle = yCurAngle - (degreesPerStep);
  }

  Serial.print(millis());
  Serial.print(",   \t");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(xAngle, 5);
  Serial.print(", ");
  Serial.print(xCurAngle, 5);
  Serial.print(",\t");
  Serial.print(y);
  Serial.print(", ");
  Serial.print(yAngle, 5);
  Serial.print(", ");
  Serial.println(yCurAngle, 5);
}
