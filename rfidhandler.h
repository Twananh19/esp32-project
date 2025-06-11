#ifndef RFIDHANDLER_H
#define RFIDHANDLER_H

#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 21

extern MFRC522 mfrc522;

void initRFID();
String readCardUID();

#endif
