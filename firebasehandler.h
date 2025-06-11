#ifndef FIREBASEHANDLER_H
#define FIREBASEHANDLER_H

#include <Firebase.h> // Firebase Client chính thức
#include <WiFi.h>
#include <secrets.h>

extern Firebase fb; // Khai báo biến fbdo để dùng chung

bool ConnectFirebase();
String getDataByUUID(const String &uuid);
int updateDataByUUID(const String &uuid, const String &newData);

#endif
