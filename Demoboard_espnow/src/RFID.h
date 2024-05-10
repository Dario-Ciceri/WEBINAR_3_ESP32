/*
   Sensore RFID/NFC
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#ifndef RFID_h
#define RFID_h

#include "Arduino.h"

class RFID
{
  public:
    void init();
    void formatToMifareClassic();
    void erase();
    void formatToNDEF();
    void read();
    String extendedRead();
    void write();
    void writeMultipleRecords();
};

#endif
