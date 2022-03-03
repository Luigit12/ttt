#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

Stepper stepper1(stepsPerRevolution, 19, 5, 18, 17);
Stepper stepper2(stepsPerRevolution, 22, 3, 1, 21);

void setup() {
  // set the speed at 5 rpm
  stepper1.setSpeed(5);
  stepper2.setSpeed(5);
  // initialize the serial port
  Serial.begin(115200);
}

void loop() {
  // step one revolution in one direction:
  Serial.println("clockwise");
  stepper1.step(stepsPerRevolution);
  stepper2.step(stepsPerRevolution);
  delay(1000);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  stepper1.step(-stepsPerRevolution);
  stepper2.step(-stepsPerRevolution);
  delay(1000);
}
