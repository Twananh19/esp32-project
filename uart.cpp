#include "uart.h"

// Chọn chân phù hợp với mạch của bạn, ví dụ RX=16, TX=17
#define UART_RX 16
#define UART_TX 17

void initUART(unsigned long baud) {
    Serial1.begin(baud, SERIAL_8N1, UART_RX, UART_TX);
}

void sendUART(const String &data) {
    Serial1.println(data);
}

String readUART() {
    if (Serial1.available()) {
        return Serial1.readStringUntil('\n');
    }
    return "";
}