#include "uart.h"

void initUART(unsigned long baud) {
    Serial.begin(baud);
}

void sendUART(const String &data) {
    Serial.println(data);
    Serial.flush();
}

String readUART() {
    if (Serial.available()) {
        return Serial.readStringUntil('\n');
    }
    return "";
}