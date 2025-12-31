#include <Servo.h>
#include <AFMotor.h>
// this is with the terminal option to choose modes ,.... but still taking only one step to voice command
#define Echo A0
#define Trig A1
#define motor 10
#define Speed 170
#define spoint 103

char value;
int distance;
String voiceCmd = "";

Servo servo;
AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);

enum Mode { MANUAL, OBSTACLE, VOICE };
Mode currentMode = MANUAL;

unsigned long lastCheck = 0;
const int checkInterval = 200;
const int stopDistance = 20;

void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  servo.attach(motor);
  servo.write(spoint);

  M1.setSpeed(Speed);
  M2.setSpeed(Speed);
  M3.setSpeed(Speed);
  M4.setSpeed(Speed);
}

void loop() {
  if (currentMode == MANUAL) {
    Bluetoothcontrol();
  } else if (currentMode == OBSTACLE) {
    Obstacle();
  } else if (currentMode == VOICE) {
    voicecontrol();
  }
}

// Manual Bluetooth Control
void Bluetoothcontrol() {
  if (Serial.available()) {
    value = Serial.read();
    Serial.println(value);
    switch (value) {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      case 'S': Stop(); break;
      case 'O': currentMode = OBSTACLE; Stop(); break;
      case 'V': currentMode = VOICE; Stop(); break;
      case 'M': currentMode = MANUAL; Stop(); break;
    }
  }
}

// Obstacle Avoidance Mode
void Obstacle() {
  unsigned long currentTime = millis();

  if (currentTime - lastCheck >= checkInterval) {
    lastCheck = currentTime;
    distance = ultrasonic();

    if (distance > stopDistance) {
      forward();
      return;
    }

    Stop();
    backward();
    delay(300);
    Stop();

    int L = leftsee();
    servo.write(spoint);
    delay(300);
    int R = rightsee();
    servo.write(spoint);
    delay(300);

    if (L > R) {
      left(); delay(500); Stop();
    } else {
      right(); delay(500); Stop();
    }

    delay(200);
  }

  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'M') {
      currentMode = MANUAL;
      Stop();
    } else if (c == 'V') {
      currentMode = VOICE;
      Stop();
    }
  }
}

// Voice Control Mode
void voicecontrol() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') continue;
    if (c == 'M') {
      currentMode = MANUAL;
      voiceCmd = "";
      Stop();
      return;
    } else if (c == 'O') {
      currentMode = OBSTACLE;
      voiceCmd = "";
      Stop();
      return;
    }
    voiceCmd += c;
    delay(2);
  }

  if (voiceCmd.length() > 0) {
    voiceCmd.toLowerCase();
    Serial.println("Voice: " + voiceCmd);

    if (voiceCmd.indexOf("forward") >= 0) {
      forward();
      while (true) {
        int d = ultrasonic();
        if (d <= stopDistance) {
          Stop();
          Serial.println("Obstacle detected!");
          break;
        }
        if (Serial.available()) {
          char interruptChar = Serial.read();
          if (interruptChar == 'M') {
            currentMode = MANUAL; Stop(); return;
          } else if (interruptChar == 'O') {
            currentMode = OBSTACLE; Stop(); return;
          }
        }
      }

    } else if (voiceCmd.indexOf("backward") >= 0) {
      backward(); delay(1000); Stop();

    } else if (voiceCmd.indexOf("left") >= 0) {
      left(); delay(500); Stop();

    } else if (voiceCmd.indexOf("right") >= 0) {
      right(); delay(500); Stop();

    } else if (voiceCmd.indexOf("stop") >= 0) {
      Stop();
    }

    voiceCmd = "";
  }
}

// Movement Functions
void forward() {
  M1.run(FORWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(FORWARD);
}

void backward() {
  M1.run(BACKWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(BACKWARD);
}

void right() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}

void left() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}

void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}

// Ultrasonic Distance
int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t / 29 / 2;
  return cm;
}

// Servo Vision
int rightsee() {
  servo.write(20);
  delay(400);
  return ultrasonic();
}

int leftsee() {
  servo.write(180);
  delay(400);
  return ultrasonic();
}