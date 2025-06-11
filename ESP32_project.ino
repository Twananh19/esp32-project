#include "wifihandler.h"
#include "secrets.h"
#include "firebasehandler.h"
#include "rfidhandler.h"
#include "realtime.h"
#include "lcdhandler.h"
#include "keypadhandler.h"
#include "servo.h"
#include <ArduinoJson.h>
#include <map>

std::map<String, int> scanCount;
// #include "servo.h"
struct CardData
{
  String uuid;
  String password;
  int money;
  String time;
};

CardData parseCardData(const String &jsonString)
{
  CardData data;

  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, jsonString);

  if (!error)
  {
    data.uuid = doc["uuid"] | "";
    data.password = doc["password"] | "";
    data.money = doc["money"] | 0;
    data.time = doc["time"] | " ";
  }
  else
  {
    Serial.print("Failed to parse JSON: ");
    Serial.println(error.f_str());
  }

  return data;
}

String lastUID = "";
unsigned long lastReadTime = 0;
const unsigned long debounceTime = 3000;

CardData checkPassword(const String &password) {
  String allData = fb.getJson("data");
  CardData found;
  if (allData.length() > 0) {
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, allData);
    if (!error) {
      for (JsonPair kv : doc.as<JsonObject>()) {
        JsonObject user = kv.value().as<JsonObject>();
        String pw = user["password"] | "";
        if (pw == password) {
          found.uuid = user["uuid"] | "";
          found.password = pw;
          found.money = user["money"] | 0;
          found.time = user["time"] | "";
          return found;
        }
      }
    }
  }
  found.password = "";
  return found;
}

void setup()
{
  Serial.begin(115200);
  bool connected = connectWiFi(WIFI_SSID, WIFI_PASSWORD);
  if (!connected)
  {
    Serial.println("Cannot proceed without WiFi.");
    // while(true);
  }
  bool connect_firebase = ConnectFirebase();
  initRFID();
  initLCD();
  initRTC();
  initServo(12); // Chỉnh lại số chân phù hợp với mạch của bạn
  initKeypad();
}

// String lastUID = "";
// unsigned long lastReadTime = 0;
// const unsigned long debounceTime = 3000;

// void loop() {
//   String cardUID = readCardUID();
//   if (cardUID != "") {
//     unsigned long now = millis();
//     if (cardUID != lastUID || now - lastReadTime > debounceTime) {
//       lastUID = cardUID;
//       lastReadTime = now;

//       String data = getDataByUUID(cardUID);
//       if (data.length() > 0) {
//         CardData card = parseCardData(data);
//         // Hiển thị thông tin và mở cổng
//         DateTime nowTime = getCurrentTime();
//         String timeStr = formatDateTime(nowTime);
//         clearLCD();
//         printLCD(0, 0, "Quet: " + timeStr);
//         printLCD(0, 1, "No: " + String(card.money));
//         openGate();
//       } else {
//         // Thẻ mới, yêu cầu đăng ký mật khẩu
//         clearLCD();
//         printLCD(0, 0, "The moi! Dang ky");
//         String password = getPasswordFromKeypad("Nhap mat khau:");
//         String json = "{\"uuid\":\"" + cardUID + "\",\"password\":\"" + password + "\",\"time\":0,\"money\":0}";
//         updateDataByUUID(cardUID, json);
//         printLCD(0, 0, "Cho mo cong tu app");
//         delay(2000);
//         clearLCD();
//       }
//     }
//   } else {
//     // Không có thẻ, cho phép nhập mật khẩu
//     printLCD(0, 0, "Nhap mat khau:");
//     String password = getPasswordFromKeypad();
//     CardData card = checkPassword(password);
//     if (card.password.length() > 0) {
//       clearLCD();
//       printLCD(0, 0, "Mo cong thanh cong");
//       printLCD(0, 1, "No: " + String(card.money));
//       openGate();
//       delay(2000);
//       clearLCD();
//     } else {
//       clearLCD();
//       printLCD(0, 0, "Sai mat khau!");
//       delay(2000);
//       clearLCD();
//     }
//   }
//   handleGate();
// }

void loop() {
  String cardUID = readCardUID();
  if (cardUID != "") {
    Serial.print("UID quet duoc: ");
    Serial.println(cardUID);
    unsigned long now = millis();
    if (cardUID != lastUID || now - lastReadTime > debounceTime) {
      lastUID = cardUID;
      lastReadTime = now;

      String data = getDataByUUID(cardUID);
      Serial.print("Data tra ve tu Firebase: ");
      Serial.println(data);
      if (data.length() > 0) {
        CardData card = parseCardData(data);
        scanCount[cardUID]++;
        Serial.print("So lan quet hien tai: ");
        Serial.println(scanCount[cardUID]);
        // Nếu đủ 2 lượt thì trừ tiền và reset đếm
        if (scanCount[cardUID] >= 2) {
          card.money = (scanCount[cardUID] / 2) * 5000; // Cộng thêm 5000
          // scanCount[cardUID] = 0;
        }
        DateTime nowTime = getCurrentTime();
        String timeStr = formatDateTime(nowTime);
        card.time = timeStr;
          // Cập nhật dữ liệu lên Firebase
        String json = "{\"uuid\":\"" + card.uuid + "\",\"password\":\"" + card.password + "\",\"money\":" + String(card.money) + ",\"time\":\"" + card.time + "\"}";
        updateDataByUUID(card.uuid, json);
        clearLCD();
        printLCD(0, 0, "Quet: " + timeStr);
        printLCD(0, 1, "No: " + String(card.money));
        openGate();
        delay(2000);
        // clearLCD();
      } else {
        clearLCD();
        printLCD(0, 0, "The chua dang ky");
        delay(2000);
        clearLCD();
      }
    }
  }
  handleGate();
}