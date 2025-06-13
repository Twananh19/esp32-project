#include "servo.h"
#include <ESP32Servo.h>
#include <Arduino.h>

Servo gateServo;
unsigned long gateOpenMillis = 0;
bool gateIsOpen = false;

void initServo(int pin) {
    gateServo.setPeriodHertz(50);
    gateServo.attach(pin, 500, 2400);
    gateServo.write(0); // Đóng cổng ban đầu
}

void openGate() {
    Serial.println("openGate called");
    gateServo.write(90); // Mở cổng
    gateOpenMillis = millis();
    gateIsOpen = true;
}

void handleGate() {
    if (gateIsOpen && millis() - gateOpenMillis > 2000) { // 2 giây
        closeGate();
        gateIsOpen = false;
    }
}

void closeGate() {
    gateServo.write(0); // Đóng cổng
}