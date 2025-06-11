#include "lcdhandler.h"

// Tùy chỉnh chân SDA và SCL theo phần cứng của bạn
#define I2C_SDA 26
#define I2C_SCL 25

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void initLCD()
{
    Wire.begin(I2C_SDA, I2C_SCL);
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void printLCD(int col, int row, const String &text)
{
    lcd.setCursor(col, row);
    lcd.print(text);
}

void clearLCD()
{
    lcd.clear();
}
