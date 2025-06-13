#ifndef UART_HANDLER_H
#define UART_HANDLER_H

#include <Arduino.h>

// Khởi tạo UART (có thể truyền tốc độ baud và chân nếu cần)
void initUART(unsigned long baud = 115200);

// Gửi chuỗi dữ liệu qua UART
void sendUART(const String &data);

// Đọc dữ liệu UART, trả về chuỗi nếu có, rỗng nếu không
String readUART();

#endif