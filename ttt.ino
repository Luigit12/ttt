#include <Stepper.h>

#include "vec.h"

#define sign(x) ((0 < x) - (x < 0))

/*
   Units:
    - distances are stored in meters
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

WebServer server(80);

const float pole_height = 530;
Vec poles_pos[3] = {Vec(0, 577, pole_height), Vec(-500, -289, pole_height),
                    Vec(500, -289, pole_height)};
Vec ttt_pos(0, 0, 0);
float rope_lengths[3] = {1000, 1000, 1000};

void setup() {
  Serial.begin(115200);

  // Setup wifi: https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
  WiFi.mode(WIFI_AP)
  WiFi.softAP("ttt system", "tttxyz");
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  if (MDNS.begin("ttt")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  
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

float pythagoras(Vec v1, Vec v2) { return distance(v1, v2); }

void calc_lengths(Vec pos, float dest_lengths[3]) {
  for (int p = 0; p < 3; p++) {
    dest_lengths[p] = pythagoras(pos, poles_pos[p]);
  }
}

void calc_delta_lengths(Vec dst, float delta_lengths[3]) {
  float dest_lengths[3];
  calc_lengths(dst, dest_lengths);

  for (int p = 0; p < 3; p++) {
    Serial.printf("Pole nr: %d, dest length: %.2f, current length: %.2f\n", p, dest_lengths[p], rope_lengths[p]);
    delta_lengths[p] = dest_lengths[p] - rope_lengths[p];
  }
}

void moveTo(Vec dst) {
  Serial.print("\n**************\nMoving to: ");
  dst.print(true);

  float delta_lengths[3];
  calc_delta_lengths(dst, delta_lengths);
  Serial.printf("Calculated delta lengths: %.2f %.2f %.2f\n", delta_lengths[0], delta_lengths[1], delta_lengths[2]);

  float delta_revs[3];

  float circumference = 2 * PI * bobbinRadius;
  for (int p = 0; p < 3; p++) {
    delta_revs[p] = delta_lengths[p] / circumference;
    rope_lengths[p] += delta_lengths[p];
  }

  Serial.printf("delta revs: %.2f, %.2f, %.2f\n", delta_revs[0], delta_revs[1],
                delta_revs[2]);
  Serial.printf("new rope lengths: %.2f, %.2f, %.2f\n", rope_lengths[0], rope_lengths[1], rope_lengths[2]);

  turnall(delta_revs[0], delta_revs[1], delta_revs[2]);
  ttt_pos.setAt(dst);
}

void loop() {
  if (Serial.available()) {
    char command[64];
    float tmp1, tmp2, tmp3;
    command[Serial.readBytesUntil('\n', command, sizeof(command))] = '\0';
    Serial.printf("got command: %s\n", command);
    
    if (sscanf(command, "turn %f %f %f", &tmp1, &tmp2, &tmp3) == 3) {
      turnall(tmp1, tmp2, tmp3);
    } else if (sscanf(command, "move %f %f %f", &tmp1, &tmp2, &tmp3) == 3) {
      moveTo(Vec(tmp1, tmp2, tmp3));
    } else if (sscanf(command, "ropes %f %f %f", &tmp1, &tmp2, &tmp3) == 3) {
      rope_lengths[0] = tmp1;
      rope_lengths[1] = tmp2;
      rope_lengths[2] = tmp3;
    }
  }

  // ttt positioning system
  // 1. position in the triangle to rope lengths
  // 2. translate rope lengths to motor revolutions
  // 3. execute revolutions simultaneously
}
