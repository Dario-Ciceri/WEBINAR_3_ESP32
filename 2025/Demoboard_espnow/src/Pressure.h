/*
   Sensore di pressione
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#ifndef Pressure_h
#define Pressure_h

#include "Arduino.h"

// Define the struct to hold sensor data
struct PressureData {
  float temperature;
  float pressure;
  float altitude;
};

class Pressure
{
  public:
    void init();
    PressureData read();
};

#endif

