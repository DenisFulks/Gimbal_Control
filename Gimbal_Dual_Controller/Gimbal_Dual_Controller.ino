#include <Stepper.h>

// pins
#define Joystick A0

// constants
float steps = 2048;
float degreesPerStep = 360 / steps;

// parameters
float maxAngle = 10;      // Sets max angle which gimbal can go
float factor = 1.15;      // Sets at which angle switches between slow and fast
int fast = 2;             // Number of steps per cycle at fast speed
int slow = 1;             // Number of steps per cycle at slow speed
int speed = 700;          // Speed of stepper

// variables
float x;
float Angle;
float Offset;
float CurAngle;
int Steps;

// Allows for mapping analog input to degree measure
float mapFloat(float x, float inMin, float inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

// Steppers
Stepper Stepper(steps / 64, 13, 11, 12, 10);

void setup() {
  Serial.begin(9600);

  pinMode(Joystick, INPUT);

  // Calibration
  Serial.println();
  Serial.print("Calibrating... Please keep the stick at a neutral position then press enter... ");
  while(Serial.available() == 0) {}

  x = analogRead(Joystick);

  Offset = mapFloat(x, 0, 1023, -1 * maxAngle, maxAngle);

  Serial.println("Done!");

  Stepper.setSpeed(speed);
}

void loop() {
  // Reads from joystick
  x = analogRead(Joystick);

  // Calculates angle based on analog input
  // Accounts for offset
  Angle = mapFloat(x, 0, 1023, -1 * maxAngle + Offset, maxAngle - Offset) - Offset;

  // Changes speed based on angle for x-input
  if (abs(Angle) < degreesPerStep && abs(CurAngle / degreesPerStep) < fast) {
    Steps = slow;
  } else if (abs(factor * CurAngle) <= abs(Angle) || abs(factor * Angle) <= abs(CurAngle)) {
    Steps = fast;
  } else {
    Steps = slow;
  }

  // Steps for x stepper
  if (abs(abs(Angle) - abs(CurAngle)) < degreesPerStep) {

  } else if (CurAngle <= Angle) {
    Stepper.step(Steps);
    CurAngle = CurAngle + (Steps * degreesPerStep);
  } else if (CurAngle >= Angle) {
    Stepper.step(-Steps);
    CurAngle = CurAngle - (Steps * degreesPerStep);
  }

  Serial.print(millis());
  Serial.print(",   \t");
  Serial.print(x);
  Serial.print(", ");
  Serial.print(Angle, 5);
  Serial.print(", ");
  Serial.println(CurAngle, 5);
}
