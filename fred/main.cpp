#include <Arduino.h>
#define USE_TIMER_2 true
#include <TimerInterrupt.h>

#include <Servo.h>

Servo frontGate;
Servo backGate;
int frontGatePin = 5;
int backGatePin = 11;
double rotationAngle;

// void timeHandler(void) {
//   rotationAngle = map(analogRead(A0), (double)0, (double)1023, 0, 180);
//   frontGate.write(rotationAngle);
// }

void setup() {
  Serial.begin(9600);
  frontGate.attach(frontGatePin);
  frontGate.write(0);
  // ITimer2.attachInterrupt(10, timeHandler);
}

void loop() {
  rotationAngle = map(analogRead(A0), (double)0, (double)1023, 0, 180);
  frontGate.write(rotationAngle);
  Serial.println(rotationAngle);
}