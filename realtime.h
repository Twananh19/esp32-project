#ifndef REALTIME_H
#define REALTIME_H

#include <RTClib.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

void initRTC();
DateTime getCurrentTime();
void setRTC(const DateTime &dt);
String getCurrentDateTime();
String formatDateTime(const DateTime &dt);
DateTime parseDateTime(const String &iso);
#endif
