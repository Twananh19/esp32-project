#ifndef SERVOHANDLER_H
#define SERVOHANDLER_H

#include <ESP32Servo.h>

void initServo(int pin);
void openGate();
void closeGate();
void handleGate(); // <-- Thêm dòng này

#endif