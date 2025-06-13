
#ifndef WEB_HANDLER_H
#define WEB_HANDLER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Khai báo web server để file khác có thể dùng
extern ESP8266WebServer server;

// Các biến toàn cục lưu thông tin thẻ
extern String lastCardUID;
extern String lastCardTime;
extern String lastCardMoney;

// Khai báo các hàm setup và xử lý
void setupServer();
void handleCardInfo();
void handleGateIn();
void handleGateOut();
// void handleGateClose(); // Thêm dòng này


#endif