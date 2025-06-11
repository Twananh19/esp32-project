#include "realtime.h"

RTC_DS3231 rtc;

void initRTC() {
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Lấy thời gian hiện tại từ RTC
  DateTime rtcNow = rtc.now();

  // Lấy thời gian từ API
  String dateTimeStr = getCurrentDateTime();
  if (dateTimeStr != "") {
    DateTime apiNow = parseDateTime(dateTimeStr);

    // In ra cả hai thời gian để kiểm tra
    Serial.print("RTC Time: ");
    Serial.println(formatDateTime(rtcNow));
    Serial.print("API Time: ");
    Serial.println(formatDateTime(apiNow));

    // So sánh độ lệch (tính bằng giây)
    long diff = std::abs((long)apiNow.unixtime() - (long)rtcNow.unixtime());
    if (rtc.lostPower() || diff > 10) {
      Serial.println("RTC time differs from API time, updating RTC...");
      rtc.adjust(apiNow);
      Serial.print("RTC Updated to: ");
      Serial.println(formatDateTime(apiNow));
    } else {
      Serial.println("RTC time is synchronized with API time.");
    }

  } else {
    Serial.println("Failed to get online time. Keeping current RTC time.");
  }
}

DateTime getCurrentTime()
{
    return rtc.now();
}
String getCurrentDateTime()
{
    const char *url = "https://www.timeapi.io/api/time/current/zone?timeZone=Asia/Ho_Chi_Minh";

    WiFiClientSecure client;
    client.setInsecure(); // Bỏ kiểm tra chứng chỉ SSL (không an toàn với production)

    HTTPClient https;
    https.begin(client, url);

    int httpCode = https.GET();
    String dateTime = "";

    if (httpCode == 200)
    {
        String payload = https.getString();
        int index = payload.indexOf("\"dateTime\":\"");
        if (index != -1)
        {
            index += strlen("\"dateTime\":\"");
            int endIndex = payload.indexOf("\"", index);
            if (endIndex != -1)
            {
                dateTime = payload.substring(index, endIndex);
            }
        }
    }
    else
    {
        Serial.print("HTTP GET failed, code: ");
        Serial.println(httpCode);
    }
    https.end();
    return dateTime;
}
DateTime parseDateTime(const String &iso)
{
    int year = iso.substring(0, 4).toInt();
    int month = iso.substring(5, 7).toInt();
    int day = iso.substring(8, 10).toInt();
    int hour = iso.substring(11, 13).toInt();
    int minute = iso.substring(14, 16).toInt();
    int second = iso.substring(17, 19).toInt();

    return DateTime(year, month, day, hour, minute, second);
}

void setRTC(const DateTime &dt)
{
    rtc.adjust(dt);
}

String formatDateTime(const DateTime &dt)
{
    char buf[25];
    sprintf(buf, "%02d:%02d:%02d",
            dt.hour(), dt.minute(), dt.second());
    return String(buf);
}

// String formatTime(const DateTime &dt)
// {
//     char buf[9];
//     sprintf(buf, "%02d:%02d:%02d", dt.hour(), dt.minute(), dt.second());
//     return String(buf);
// }