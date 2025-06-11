#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>

bool connectWiFi(const char *ssid, const char *password, unsigned long timeout = 50000);

#endif
