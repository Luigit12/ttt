#include <MultiStepper.h>

// IN1 IN3 IN2 IN4
Stepper stepper1(stepsPerRevolution, 13, 14, 12, 27);
Stepper stepper2(stepsPerRevolution, 26, 33, 25, 32);
Stepper stepper3(stepsPerRevolution, 15, 4, 2, 16);

void setup() {


  Serial.begin(9600);
  Serial.println("ttt started");
}


void loop() {
  if (Serial.available()) {
    char command[64];
    float rev1, rev2, rev3;
    command[Serial.readBytesUntil('\n', command, sizeof(command))] = '\0';
    Serial.print("got command: ");
    Serial.println(command);
    if (sscanf(command, "turn %f %f %f", &rev1, &rev2, &rev3) == 3) {

    }
  }

  // ttt positioning system
  // 1. position in the triangle to rope lengths
  // 2. translate rope lengths to motor revolutions
  // 3. execute revolutions simultaneously
}
