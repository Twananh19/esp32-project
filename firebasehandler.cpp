#include "firebasehandler.h"
#include "secrets.h"
Firebase fb(REFERENCE_URL, AUTH_TOKEN);
bool ConnectFirebase()
{
    int httpCode = fb.getInt("ping");
    if (httpCode == 200)
    {
        Serial.println("Firebase connection successful.");
        return true;
    }
    else
    {
        Serial.println("Firebase connection failed.");
        return false;
    }
    Serial.println("Firebase initialized.");
}

// Lấy dữ liệu theo UUID (đường dẫn: "data/<uuid>")
// Trả về chuỗi JSON hoặc chuỗi rỗng nếu không lấy được
String getDataByUUID(const String &uuid)
{
    String path = "data/" + uuid;
    String data = fb.getJson(path);
    if (data.length() > 0)
    {
        return data;
    }
    else
    {
        Serial.print("No data found for UUID ");
        Serial.println(uuid);
    }
    return "";
}

// Cập nhật dữ liệu theo UUID (đường dẫn: "data/<uuid>")
// newData là chuỗi JSON hoặc dữ liệu dạng String để lưu
// Trả về mã HTTP, 200 nếu thành công
int updateDataByUUID(const String &uuid, const String &newData)
{
    String path = "data/" + uuid;
    int res = fb.setJson(path, newData);
    if (res == 200)
    {
        Serial.print("Data updated for UUID ");
        Serial.println(uuid);
    }
    else
    {
        Serial.print("Failed to update data for UUID ");
        Serial.print(uuid);
        Serial.print(". HTTP code: ");
        Serial.println(res);
    }
    return res;
}
