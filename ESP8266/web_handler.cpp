#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#include "web_handler.h"
#include "html_page.h"
#include "uart.h"

#define FIREBASE_HOST "https://nhungproject-747ef-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "s1lPf9qpewC0K1FwrvsGv0aFXOHBGTPxqc5v3iRy"

FirebaseData fbdo;
// FirebaseConfig config;
// FirebaseAuth auth;

// config.host = FIREBASE_HOST;
// config.signer.tokens.legacy_token = FIREBASE_AUTH;


const char *ssid = "ESP32";
const char *password = "88888888";

ESP8266WebServer server(80);
const int LED_PIN = 2;

void handleListCards() {
    String json = "[";
    if (Firebase.getJSON(fbdo, "/data")) {
        FirebaseJson &jsonObj = fbdo.jsonObject();
        size_t len = jsonObj.iteratorBegin();
        for (size_t i = 0; i < len; i++) {
            String key, value;
            int type = 0;
            jsonObj.iteratorGet(i, type, key, value);
            FirebaseJson cardJson;
            cardJson.setJsonData(value);
            FirebaseJsonData result;
            String uuid, time;
            int money = 0;
            cardJson.get(result, "uuid");
            if(result.success) uuid = result.stringValue;

            cardJson.get(result, "money");
            if(result.success) money = result.intValue;

            cardJson.get(result, "time");
            if(result.success) time = result.stringValue;

            if (i > 0) json += ",";
            json += "{\"uuid\":\"" + uuid + "\",\"money\":" + String(money) + ",\"time\":\"" + time + "\"}";
        }
        jsonObj.iteratorEnd();
    }
    json += "]";
    server.send(200, "application/json", json);
}

void setupServer()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    FirebaseConfig config;
    FirebaseAuth auth;
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // Thêm dòng này để trả về giao diện web
    server.on("/", []() {
        server.send(200, "text/html", html_page);
    });

    server.on("/cardinfo", handleCardInfo);
    server.on("/gate/IN", handleGateIn);
    server.on("/gate/OUT", handleGateOut);
    server.on("/listcards", handleListCards);
    // server.on("/gate/CLOSE", handleGateClose); // Thêm dòng này

    server.begin();
    Serial.println("Webserver started");
}
String lastCardUID = "";
String lastCardTime = "";
String lastCardMoney = "";
String lastWebMessage = "";
bool lastCardIsNew = false; // Thêm biến này

void handleCardInfo()
{
    // String json = "{\"uuid\":\"" + lastCardUID + "\",\"time\":\"" + lastCardTime + "\",\"money\":\"" + lastCardMoney + "\",\"isNew\":" + (lastCardIsNew ? "true" : "false") + "}";
    // server.send(200, "application/json", json);
    String json;
    if (lastWebMessage.startsWith("phát hiện UUID mới: ")) {
        String uid = lastWebMessage.substring(21);
        json = "{\"isNew\":true,\"uuid\":\"" + uid + "\"}";
    } else if (lastWebMessage.endsWith("đang qua cổng")) {
        int idx = lastWebMessage.indexOf(" đang qua cổng");
        String uid = lastWebMessage.substring(0, idx);
        json = "{\"isNew\":false,\"uuid\":\"" + uid + "\"}";
    } else {
        json = "{\"isNew\":false,\"uuid\":\"\"}";
    }
    server.send(200, "application/json", json);
}

void handleGateIn()
{
    sendUART("IN");
    server.send(200, "text/plain", "Đã gửi lệnh mở cổng vào!");
}

void handleGateOut()
{
    sendUART("OUT");
    server.send(200, "text/plain", "Đã gửi lệnh mở cổng ra!");
}

// void handleGateClose()
// {
//     sendUART("CLOSE");
//     server.send(200, "text/plain", "Đã gửi lệnh đóng cổng!");
// }
void handleRegister()
{
    if (lastWebMessage.startsWith("phát hiện UUID mới: ")) {
        String uid = lastWebMessage.substring(21);
        sendUART("REGISTER:" + uid);
        lastWebMessage = ""; // Reset sau khi đăng ký
        server.send(200, "text/plain", "Đã gửi lệnh đăng ký & mở cổng cho UID: " + uid);
    } else {
        server.send(400, "text/plain", "Không có UID mới để đăng ký");
    }
}

