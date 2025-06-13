#include "web_handler.h"
#include "uart.h"

extern bool lastCardIsNew;

void setup() {
  Serial.begin(9600);
  initUART(9600);
  setupServer();
}

void loop() {
  // Nhận dữ liệu từ ESP32 qua UART (giả sử ESP32 gửi JSON)
  String msg = readUART();
  
  if (msg.startsWith("{\"uuid\":")) {
    // Parse thủ công, hoặc dùng ArduinoJson nếu muốn
    int uidStart = msg.indexOf("\"uuid\":\"") + 7;
    int uidEnd = msg.indexOf("\"", uidStart);
    lastCardUID = msg.substring(uidStart, uidEnd);

    int timeStart = msg.indexOf("\"time\":\"") + 8;
    int timeEnd = msg.indexOf("\"", timeStart);
    lastCardTime = msg.substring(timeStart, timeEnd);

    int moneyStart = msg.indexOf("\"money\":\"") + 9;
    int moneyEnd = msg.indexOf("\"", moneyStart);
    lastCardMoney = msg.substring(moneyStart, moneyEnd);
    lastCardIsNew = (lastCardMoney == "0");
  }
  server.handleClient();
}