#include <Stepper.h>

#include "vec.h"

#define sign(x) ((0 < x) - (x < 0))

/*
   Units:
    - distances are (of course) stored in meters
    - times are stored in seconds
*/


const int speed = 15;
const int stepsPerRevolution = 2048;
const int bobbinRadius = 7;
const int ropePerRevolution = 2 * PI * bobbinRadius;

// ESP32 devkit pinout:
// https://www.mischianti.org/wp-content/uploads/2020/11/ESP32-DOIT-DEV-KIT-v1-pinout-mischianti.png
// IN1 IN3 IN2 IN4
Stepper stepper1(stepsPerRevolution, 13, 14, 12, 27);
Stepper stepper2(stepsPerRevolution, 26, 33, 25, 32);
Stepper stepper3(stepsPerRevolution, 15, 4, 2, 16);

const float pole_height = 100;
Vec poles_pos[3] = {Vec(0, 577, pole_height), Vec(-0.5, -289, pole_height), Vec(0.5, -289, pole_height)};
Vec ttt_pos(0, 0, 0);
float rope_lengths[3] = {1000, 1000, 1000};

void setup() {
  Serial.begin(115200);

  stepper1.setSpeed(speed);
  stepper2.setSpeed(speed);
  stepper3.setSpeed(speed);
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

float pythagoras(Vec v1, Vec v2) {
  return distance(v1, v2);
}

void calc_lengths(Vec pos, float dest_lengths[3]) {
  for (int p = 0; p < 3; p++) {
    dest_lengths[p] = pythagoras(pos, poles_pos[p]);
  }
}

void calc_delta_lengths(Vec dst, float delta_lengths[3]) {
  float dest_lengths[3];
  calc_lengths(dst, dest_lengths);

  for (int p = 0; p < 3; p++) {
    delta_lengths[p] = dest_lengths[p] - rope_lengths[p];
  }
}

void moveTo(Vec dst) {
  float delta_lengths[3];
  calc_delta_lengths(dst, delta_lengths);

  float delta_revs[3];

  float circumference = 2 * PI * bobbinRadius;
  for (int p = 0; p < 3; p++) {
    delta_revs[p] = delta_lengths[p] / circumference;
  }
}

void loop() {
  if (Serial.available()) {
    char command[64];
    float rev1, rev2, rev3;
    float destx, desty, destz;
    command[Serial.readBytesUntil('\n', command, sizeof(command))] = '\0';
    Serial.print("got command: ");
    Serial.println(command);
    if (sscanf(command, "turn %f %f %f", &rev1, &rev2, &rev3) == 3) {
      turnall(rev1, rev2, rev3);
    } else if (sscanf(command, "moveto %f %f %f", &destx, &desty, &destz) == 3) {
      moveTo(Vec(destx, desty, destz));
    }
  }

  // ttt positioning system
  // 1. position in the triangle to rope lengths
  // 2. translate rope lengths to motor revolutions
  // 3. execute revolutions simultaneously
}
