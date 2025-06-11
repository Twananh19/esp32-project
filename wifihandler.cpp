#include "wifihandler.h"
#include <Arduino.h>

bool connectWiFi(const char *ssid, const char *password, unsigned long timeout)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); // Ngắt kết nối cũ nếu có
    delay(100);

    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();

    // Đợi kết nối hoặc timeout
    while (WiFi.status() != WL_CONNECTED && (millis() - startAttemptTime) < timeout)
    {
        Serial.print(".");
        delay(500);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println();
        Serial.print("Connected! IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    else
    {
        Serial.println();
        Serial.println("Failed to connect to WiFi.");
        return false;
    }
}
