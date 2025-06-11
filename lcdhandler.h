#ifndef LCDHANDLER_H
#define LCDHANDLER_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

extern LiquidCrystal_I2C lcd;

void initLCD();
void printLCD(int col, int row, const String &text);
void clearLCD();

#endif