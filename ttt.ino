#include <Stepper.h>

#define sign(x) ((0 < x) - (x < 0))

const int speed = 15;
const int stepsPerRevolution =
    2048;  // change this to fit the number of steps per revolution

// ESP32 devkit pinout:
// https://www.mischianti.org/wp-content/uploads/2020/11/ESP32-DOIT-DEV-KIT-v1-pinout-mischianti.png
// IN1 IN3 IN2 IN4
Stepper stepper1(stepsPerRevolution, 13, 14, 12, 27);
Stepper stepper2(stepsPerRevolution, 26, 33, 25, 32);
Stepper stepper3(stepsPerRevolution, 15, 4, 2, 16);

void setup() {
  stepper1.setSpeed(speed);
  stepper2.setSpeed(speed);
  stepper3.setSpeed(speed);

  Serial.begin(9600);
  Serial.println("ttt started");
}

void turnall(float rev1, float rev2, float rev3) {
  Serial.print("turning: ");
  Serial.print(rev1);
  Serial.print(", ");
  Serial.print(rev2);
  Serial.print(", ");
  Serial.println(rev3);

  int target1 = fabs(rev1) * stepsPerRevolution;
  int target2 = fabs(rev2) * stepsPerRevolution;
  int target3 = fabs(rev3) * stepsPerRevolution;
  int dir1 = sign(rev1);
  int dir2 = sign(rev2);
  int dir3 = sign(rev3);

  int steps1 = 0;
  int steps2 = 0;
  int steps3 = 0;

  int max_target = max(max(target1, target2), target3);

  for (int i = 0; i < max_target; i++) {
    if (steps1 * max_target < i * target1) {
      stepper1.step(dir1);
      steps1++;
    }
    if (steps2 * max_target < i * target2) {
      stepper2.step(dir2);
      steps2++;
    }
    if (steps3 * max_target < i * target3) {
      stepper3.step(dir3);
      steps3++;
    }
  }
  Serial.println("done");
}

void loop() {
  if (Serial.available()) {
    char command[64];
    float rev1, rev2, rev3;
    command[Serial.readBytesUntil('\n', command, sizeof(command))] = '\0';
    Serial.print("got command: ");
    Serial.println(command);
    if (sscanf(command, "turn %f %f %f", &rev1, &rev2, &rev3) == 3) {
      turnall(rev1, rev2, rev3);
    }
  }

  // ttt positioning system
  // 1. position in the triangle to rope lengths
  // 2. translate rope lengths to motor revolutions
  // 3. execute revolutions simultaneously
}
