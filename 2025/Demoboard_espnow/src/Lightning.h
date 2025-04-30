/*
   Sensore fulmini
   Futura Academy: https://academy.futuranet.it/
   Webinar 2: Esp32 Futura Demoboard
   Dario Ciceri
   Canale YouTube: https://www.youtube.com/channel/UCuPuHsNjWX7huiztYu9ROQA
   Profilo Instagram: https://www.instagram.com/_dario.ciceri_/
*/

#ifndef Lightning_h
#define Lightning_h

#include "Arduino.h"

class Lightning
{
  public:
    byte noiseFloor;
    byte watchDogVal;
    byte spike;
    byte lightningThresh;
    byte intVal;
    int strikeDistance;
    long strikeEnergy;
    void init();
    void read();
};

#endif
