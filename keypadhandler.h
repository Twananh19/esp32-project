#ifndef KEYPADHANDLER_H
#define KEYPADHANDLER_H

#include <Arduino.h>
#include <Keypad.h>

#define PASSWORD_LENGTH 7 // 6 ký tự + null terminator

void initKeypad();
String getPasswordFromKeypad(const String &promptOnLCD = "");
void clearKeypadBuffer();

#endif