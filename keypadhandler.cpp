#include <Arduino.h>
#include <Keypad.h>
#include "keypadhandler.h"
#include "lcdhandler.h"

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};
byte rowPins[ROWS] = {14, 27, 32, 33}; // Chỉnh chân theo phần cứng
byte colPins[COLS] = {2, 4, 16, 17};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void initKeypad() {
    // Không cần khởi tạo gì thêm
}

String getPasswordFromKeypad(const String &promptOnLCD) {
    String password = "";
    char key;
    if (promptOnLCD.length() > 0) {
        clearLCD();
        printLCD(0, 0, promptOnLCD);
        printLCD(0, 1, "Nhap: ");
    }
    while (password.length() < PASSWORD_LENGTH - 1) {
        key = keypad.getKey();
        if (key) {
            if (key == '#') break; // Kết thúc nhập
            if (key == '*') {
                if (password.length() > 0) {
                    password.remove(password.length() - 1);
                    printLCD(6 + password.length(), 1, " ");
                }
            } else if (isDigit(key)) {
                password += key;
                printLCD(6 + password.length() - 1, 1, "*");
            }
        }
        delay(50);
    }
    delay(300);
    clearLCD();
    return password;
}

void clearKeypadBuffer() {
    while (keypad.getKey()) {
        delay(10);
    }
} // 👈 ĐÃ BỔ SUNG dấu } bị thiếu ở đây
