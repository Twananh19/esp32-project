#include "rfidhandler.h"
#include <SPI.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);

void initRFID()
{
    SPI.begin();
    mfrc522.PCD_Init();
    Serial.println("RFID initialized.");
}

String readCardUID()
{
    // Serial.println("Dang kiem tra the RFID...");
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
        return ""; // Không có thẻ
    }

    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();
    Serial.print("UID tu readCardUID(): ");
    Serial.println(uid);


    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    return uid;
}
